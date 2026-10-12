#include "postrender.h"
#include "../gui/framework.h"

PostRender::PostRender_t PostRender::PostRenderOriginal = nullptr;

static SDK::UMaterial* cachedMaterial = nullptr;

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

void PostRender::Hooked_PostRender(SDK::UGameViewportClient* viewport, SDK::UCanvas* canvas) {
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

    if (canvas) {
        Globals::canvas = canvas;

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

                    if (!cachedMaterial) {
                        cachedMaterial = SDK::UObject::FindObject<SDK::UMaterial>("Material WireframeMaterial.WireframeMaterial");
                    }

                    if (cachedMaterial) {
                        SDK::USkeletalMeshComponent* skeletalMesh = player->Mesh;
                        if (skeletalMesh) {
                            SDK::TArray<SDK::UMaterialInterface*> materials = skeletalMesh->GetMaterials();
                            for (int32_t j = 0; j < materials.Num(); j++) {
                                if (!materials.IsValidIndex(j)) {
                                    continue;
                                }

                                SDK::UMaterialInstanceDynamic* dynMat = skeletalMesh->CreateDynamicMaterialInstance(j, cachedMaterial, SDK::FName());
                                if (!dynMat) {
                                    continue;
                                }

                                skeletalMesh->SetMaterial(j, dynMat);
                            }
                        }
                    }
                }
            }
        }
    }

    return PostRenderOriginal(viewport, canvas);
}