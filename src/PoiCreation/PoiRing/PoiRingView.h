// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#pragma once

#include "PoiCreation.h"
#include "VectorMath.h"
#include "Camera.h"
#include "IRenderableFilter.h"
#include "Rendering.h"

namespace ExampleApp
{
    
    namespace PoiCreation
    {
        namespace PoiRing
        {
            class PoiRingView : public Eegeo::Rendering::IRenderableFilter
            {
            public:
                PoiRingView(PoiRingRenderable& poiRingRenderable,
                            Eegeo::Rendering::Renderables::BatchedSpriteRenderable& iconRenderable);
                
                ~PoiRingView();
                
                void SetRingTransforms(const Eegeo::m44& sphereMvp, const Eegeo::m44& quadMvp) const;
                void SetInnerSphereScale(const float scale);
                
                void EnqueueRenderables(const Eegeo::Rendering::RenderContext& renderContext, Eegeo::Rendering::RenderQueue& renderQueue);
                void SetShouldRenderRing(bool shouldRenderRing);
                
                void AddIconSprite(const Eegeo::Camera::RenderCamera& renderCamera, const Eegeo::dv3& position, float scale);
         
            private:
                PoiRingRenderable& m_poiRingRenderable;
                Eegeo::Rendering::Renderables::BatchedSpriteRenderable& m_iconRenderable;
                
                bool m_shouldRenderRing;
            };
        }
    }
}