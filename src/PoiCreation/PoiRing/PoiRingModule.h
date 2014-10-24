// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#pragma once

#include "PoiCreation.h"
#include "IPoiRingModule.h"
#include "Modules.h"
#include "Rendering.h"
#include "IdTypes.h"
#include "Terrain.h"
#include "GLHelpers.h"

namespace ExampleApp
{
    namespace PoiCreation
    {
        namespace PoiRing
        {
            class PoiRingModule : public IPoiRingModule
            {
            public:
                PoiRingModule(IPoiCreationModel& poiCreationModel,
                              Eegeo::Modules::IPlatformAbstractionModule& platformAbstractions,
                              Eegeo::Modules::Core::RenderingModule& renderingModule,
                              Eegeo::Modules::Core::AsyncLoadersModule& asyncLoadersModule,
                              Eegeo::Modules::Core::LightingModule& lightingModule,
                              Eegeo::Modules::Map::Layers::TerrainModelModule& terrainModelModule,
                              const Eegeo::Rendering::ScreenProperties& screenProperties);
                
                ~PoiRingModule();
                
                IPoiRingController& GetPoiRingController() const;
                IPoiRingTouchController& GetPoiRingTouchController() const;

                
            private:
                PoiRingView* m_pPoiRingView;
                PoiRingRenderable* m_pPoiRingRenderable;
                Eegeo::Rendering::Shaders::BatchedSpriteShader* m_pSpriteShader;
                Eegeo::Rendering::Materials::BatchedSpriteMaterial* m_pPoiRingIconMaterial;
                Eegeo::Rendering::Renderables::BatchedSpriteRenderable* m_pPoiRingIconRenderable;
                
                Eegeo::Helpers::GLHelpers::TextureInfo m_poiRingIconTexture;
                
                Eegeo::Resources::Terrain::Collision::TerrainRayPicker* m_pTerrainRayPicker;
                Eegeo::Rendering::RenderableFilters& m_renderableFilters;
                
                IPoiRingController* m_pPoiRingController;
                IPoiRingTouchController* m_pPoiRingTouchController;
            };
        }
    }
}