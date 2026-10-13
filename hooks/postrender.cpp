#include "postrender.h"
#include "../gui/framework.h"

PostRender::PostRender_t PostRender::PostRenderOriginal = nullptr;

static SDK::UMaterial* chamsMaterial = nullptr;

void DumpMaterials() {
    auto objects = SDK::UObject::GObjects.GetTypedPtr();
    if (!objects) {
        return;
    }

    int count = 0;

    for (int32_t i = 0; i < objects->Num(); i++) {
        SDK::UObject* obj = objects->GetByIndex(i);
        if (!obj) {
            continue;
        }

        if (obj->IsA(SDK::UMaterial::StaticClass()) || obj->IsA(SDK::UMaterialInstanceConstant::StaticClass())) {
            std::string objName = obj->GetFullName();

            std::println("{}", objName.c_str());

            count++;
        }
    }

    std::println("finished dump! total materials found: {}", count);
}

bool K2_Project(SDK::FVector worldLocation, SDK::FVector2D* screenLocation) {
    SDK::FVector returnValue = Globals::canvas->K2_Project(worldLocation);
    if (returnValue.Z <= 0.0) {
        returnValue.X *= -1.0;
        returnValue.Y *= -1.0;

        returnValue.X += Globals::screenSize.X;
        returnValue.Y += Globals::screenSize.Y;
    }

    if (returnValue.Z > 0.0) {
        *screenLocation = SDK::FVector2D(returnValue.X, returnValue.Y);
        return true;
    }

    return false;
}

void PostRender::Hooked_PostRender(SDK::UGameViewportClient* viewport, SDK::UCanvas* canvas) {
    Globals::canvas = canvas;
    Globals::screenSize = SDK::FVector2D(double(canvas->ClipX), double(canvas->ClipY));

    SDK::UWorld* world = viewport->World;
    if (!world) {
        return PostRenderOriginal(viewport, canvas);
    }

    SDK::ULevel* persistentLevel = world->PersistentLevel;
    if (!persistentLevel) {
        return PostRenderOriginal(viewport, canvas);
    }

    SDK::UGameInstance* gameInstance = viewport->GameInstance;
    if (!gameInstance) {
        return PostRenderOriginal(viewport, canvas);
    }

    SDK::ULocalPlayer* localPlayer = gameInstance->LocalPlayers[0];
    if (!localPlayer) {
        return PostRenderOriginal(viewport, canvas);
    }

    SDK::APlayerController* localController = localPlayer->PlayerController;
    if (!localController) {
        return PostRenderOriginal(viewport, canvas);
    }

    if (GetAsyncKeyState(VK_INSERT) & 1) {
        GUI::open = !GUI::open;
    }

    //if (GetAsyncKeyState(VK_ESCAPE) & 1) {
    //    DumpMaterials();
    //}

    GUI::DrawTxt(SDK::FVector2D(10.0f, 10.0f), SDK::FString(L"jewengine5 NIGGAAAAA"));

    if (GUI::open) {
        GUI::BeginWindow(SDK::FVector2D(100.0f, 100.0f), SDK::FVector2D(300.0f, 400.0f), SDK::FString(L"jewengine5"));

        GUI::Checkbox(SDK::FString(L"Bounding Box"), &Features::boundingBox);
    }

    if (persistentLevel && localController->AcknowledgedPawn) {
        SDK::TArray<SDK::AActor*> actors = persistentLevel->Actors;
        for (int i = 0; i < actors.Num(); i++) {
            SDK::AActor* actor = actors[i];
            if (!actor) {
                continue;
            }

            if (actor->IsA(SDK::EClassCastFlags::Pawn)) {
                SDK::ABP_PlayerCharacter_C* player = static_cast<SDK::ABP_PlayerCharacter_C*>(actor);
                if (!player) {
                    continue;
                }

                SDK::ABP_PlayerState_C* playerState = static_cast<SDK::ABP_PlayerState_C*>(player->PlayerState);
                if (!playerState) {
                    continue;
                }

                if (player == localController->AcknowledgedPawn) {
                    continue;
                }

                if (player->bDead) {
                    continue;
                }

                SDK::USkeletalMeshComponent* skeletalMesh = player->Mesh;
                if (!skeletalMesh) {
                    continue;
                }

                SDK::FVector origin, extent = SDK::FVector();
                float sphereRadius = 0.0f;

                SDK::UKismetSystemLibrary::GetComponentBounds(skeletalMesh, &origin, &extent, &sphereRadius);

                double mostLeft = DBL_MAX;
                double mostRight = DBL_MIN;
                double mostTop = DBL_MAX;
                double mostBottom = DBL_MIN;

                for (int x = 0; x < 8; x++) {
                    SDK::FVector corner = SDK::FVector();

                    corner.X = (x & 1) ? extent.X : -extent.X;
                    corner.Y = (x & 2) ? extent.Y : -extent.Y;
                    corner.Z = (x & 2) ? extent.Z : -extent.Z;

                    SDK::FVector2D origin2d = SDK::FVector2D();
                    if (!K2_Project(origin + corner, &origin2d)) {
                        continue;
                    }

                    if (origin2d.X < mostLeft) {
                        mostLeft = origin2d.X;
                    }

                    if (origin2d.X > mostRight) {
                        mostRight = origin2d.X;
                    }

                    if (origin2d.Y < mostTop) {
                        mostTop = origin2d.Y;
                    }

                    if (origin2d.Y > mostBottom) {
                        mostBottom = origin2d.Y;
                    }
                }

                double actorHeight = mostBottom - mostTop;
                double actorWidth = mostRight - mostLeft;

                Globals::canvas->K2_DrawBox(SDK::FVector2D(mostLeft, mostTop), SDK::FVector2D(actorWidth, actorHeight), 1.0f, SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

                if (!chamsMaterial) {
                    chamsMaterial = SDK::UObject::FindObject<SDK::UMaterial>("Material WireframeMaterial.WireframeMaterial");
                }

                if (chamsMaterial) {
                    chamsMaterial->bDisableDepthTest = true;
                    chamsMaterial->BlendMode = SDK::EBlendMode::BLEND_Additive;

                    skeletalMesh->bRenderCustomDepth = true;
                    skeletalMesh->SetCustomDepthStencilValue(11);

                    SDK::TArray<SDK::UMaterialInterface*> materials = skeletalMesh->GetMaterials();
                    for (int32_t j = 0; j < materials.Num(); j++) {
                        if (!materials.IsValidIndex(j)) {
                            continue;
                        }

                        SDK::UMaterialInstanceDynamic* dynMat = skeletalMesh->CreateDynamicMaterialInstance(j, chamsMaterial, SDK::FName());
                        if (!dynMat) {
                            continue;
                        }

                        SDK::FLinearColor renderColor = SDK::FLinearColor(4.0f, 0.0f, 4.0f, 1.0f);

                        dynMat->SetVectorParameterValue(SDK::UKismetStringLibrary::Conv_StringToName(L"EmissiveColor"), renderColor);
                        dynMat->SetVectorParameterValue(SDK::UKismetStringLibrary::Conv_StringToName(L"Color"), renderColor);

                        skeletalMesh->SetMaterial(j, dynMat);
                    }
                }
            }
        }
    }

    return PostRenderOriginal(viewport, canvas);
}