// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "PoiRingTouchController.h"
#include "CameraHelpers.h"
#include "RenderCamera.h"
#include "IMyPinCreationModel.h"
#include "GlobeCameraController.h"
#include "EarthConstants.h"
#include "IntersectionTests.h"
#include "IPoiRingController.h"
#include "IAppModeModel.h"
#include "InteriorController.h"
#include "InteriorsFloorModel.h"
#include "Bounds.h"
#include "InteriorsModel.h"
#include "EcefTangentBasis.h"
#include "InteriorHeightHelpers.h"

namespace ExampleApp
{
    namespace MyPinCreation
    {
        namespace PoiRing
        {
            namespace SdkModel
            {
                PoiRingTouchController::PoiRingTouchController(MyPinCreation::SdkModel::IMyPinCreationModel& myPinCreationModel,
                                                               Eegeo::Collision::IRayPicker& rayPicker,
                                                               const IPoiRingController& poiRingController,
                                                               ExampleApp::AppModes::SdkModel::IAppModeModel& appModeModel,
                                                               Eegeo::Resources::Interiors::InteriorController& interiorController)
                    : m_myPinCreationModel(myPinCreationModel)
                    , m_rayPicker(rayPicker)
                    , m_poiRingController(poiRingController)
                    , m_isDragging(false)
                    , m_appModeModel(appModeModel)
                    , m_interiorController(interiorController)
                {

                }

                bool PoiRingTouchController::HandleTouchDown(const AppInterface::TouchData& data, const Eegeo::Camera::RenderCamera& renderCamera, const Eegeo::dv3& nonFlattenedCameraPosition)
                {
                    if (m_myPinCreationModel.GetCreationStage() != Ring)
                    {
                        return false;
                    }

                    float screenPixelX = data.point.GetX();
                    float screenPixelY = data.point.GetY();

                    Eegeo::dv3 rayDirection;
                    Eegeo::Camera::CameraHelpers::GetScreenPickRay(renderCamera, screenPixelX, screenPixelY, rayDirection);

                    Eegeo::dv3 rayOrigin = nonFlattenedCameraPosition;
                    Eegeo::dv3 rayIntersectionPoint;
                    double intersectionParam;
                    float terrainHeight;
                    float heightAboveTerrain;
                    bool rayPick = PerformRayPick(rayOrigin, rayDirection, rayIntersectionPoint, intersectionParam, terrainHeight, heightAboveTerrain);

                    if (rayPick)
                    {
                        Eegeo::dv3 iconPosition;
                        float iconSize;
                        m_poiRingController.GetIconPositionAndSize(iconPosition, iconSize);
                        
                        Eegeo::dv3 spherePosition;
                        float sphereRadius;
                        m_poiRingController.GetSpherePositionAndRadius(spherePosition, sphereRadius);

                        bool hitIcon = Eegeo::Geometry::IntersectionTests::TestRaySphere(rayOrigin, rayDirection, iconPosition, iconSize/2.0f);
                        if ((rayIntersectionPoint - spherePosition).Length() < sphereRadius || hitIcon)
                        {
                            m_dragOffset = rayIntersectionPoint - spherePosition;
                            m_isDragging = true;
                            return true;
                        }
                    }

                    return false;
                }

                bool PoiRingTouchController::HandleTouchUp(const AppInterface::TouchData& data)
                {
                    m_isDragging = false;
                    m_dragOffset = Eegeo::dv3();

                    if (m_myPinCreationModel.GetCreationStage() != Ring)
                    {
                        return false;
                    }

                    return true;
                }

                bool PoiRingTouchController::HandleTouchMove(const AppInterface::TouchData &data, const Eegeo::Camera::RenderCamera &renderCamera, const Eegeo::dv3& nonFlattenedCameraPosition)
                {
                    if (m_myPinCreationModel.GetCreationStage() != Ring)
                    {
                        return false;
                    }

                    if (m_isDragging)
                    {
                        float screenPixelX = data.point.GetX();
                        float screenPixelY = data.point.GetY();

                        Eegeo::dv3 rayDirection;
                        Eegeo::Camera::CameraHelpers::GetScreenPickRay(renderCamera, screenPixelX, screenPixelY, rayDirection);

                        Eegeo::dv3 rayOrigin = nonFlattenedCameraPosition;
                        Eegeo::dv3 rayIntersectionPoint;
                        double intersectionParam;
                        float terrainHeight;
                        float heightAboveTerrain;
                        bool rayPick = PerformRayPick(rayOrigin, rayDirection, rayIntersectionPoint, intersectionParam, terrainHeight, heightAboveTerrain);

                        if (rayPick)
                        {
                            Eegeo::Space::LatLong latLong = Eegeo::Space::LatLong::FromECEF(rayIntersectionPoint - m_dragOffset);
                            m_myPinCreationModel.SetLatLong(latLong);
                            m_myPinCreationModel.SetTerrainHeight(terrainHeight);
                            m_myPinCreationModel.SetHeightAboveTerrain(heightAboveTerrain);
                        }

                        return true;
                    }

                    return false;
                }

                bool PoiRingTouchController::IsDragging() const
                {
                    return m_isDragging && m_myPinCreationModel.GetCreationStage() == Ring;
                }

                bool PoiRingTouchController::PerformRayPick(const Eegeo::dv3 &rayOrigin,
                                                            Eegeo::dv3 &rayDirection,
                                                            Eegeo::dv3 &out_rayIntersectionPoint,
                                                            double &out_intersectionParam,
                                                            float &out_terrainHeight,
                                                            float &out_heightAboveTerrain)
                {
                    bool rayPick = false;

                    if(m_appModeModel.GetAppMode() == AppModes::SdkModel::InteriorMode && m_interiorController.InteriorIsVisible())
                    {
                        const Eegeo::Resources::Interiors::InteriorsModel* interiorsModel;
                        
                        Eegeo_ASSERT(m_interiorController.TryGetCurrentModel(interiorsModel), "Couldn't get current interiorsModel");
                        
                        const Eegeo::dv3 originNormal = interiorsModel->GetTangentBasis().GetUp();

                        float floorHeightAboveSeaLevel = Helpers::InteriorHeightHelpers::GetFloorHeightAboveSeaLevel(*interiorsModel, m_interiorController.GetCurrentFloorIndex());
                        
                        const Eegeo::dv3 point = originNormal * (floorHeightAboveSeaLevel + Eegeo::Space::EarthConstants::Radius);
                        
                        out_terrainHeight = interiorsModel->GetTangentSpaceBounds().GetMin().y;
                        out_heightAboveTerrain = floorHeightAboveSeaLevel - out_terrainHeight;
                        rayPick = Eegeo::Geometry::IntersectionTests::RayIntersectsWithPlane(rayOrigin, rayDirection, originNormal, point, out_intersectionParam, out_rayIntersectionPoint);
                    }
                    else
                    {
                        rayPick = m_rayPicker.TryGetRayIntersection(rayOrigin, rayDirection, out_rayIntersectionPoint, out_intersectionParam);
                        if(rayPick)
                        {
                            out_terrainHeight = static_cast<float>(out_rayIntersectionPoint.Length() - Eegeo::Space::EarthConstants::Radius);
                            out_heightAboveTerrain = 0.0f;
                        }
                    }
                    if(!rayPick)
                    {
                        rayPick = Eegeo::Geometry::IntersectionTests::GetRayEarthSphereIntersection(rayOrigin, rayDirection, out_rayIntersectionPoint, Eegeo::Space::EarthConstants::RadiusSquared);
                        if(rayPick)
                        {
                            out_terrainHeight = 0.0f;
                            out_heightAboveTerrain = 0.0f;
                            out_intersectionParam = (out_rayIntersectionPoint - rayOrigin).Length();
                        }
                    }

                    return rayPick;
                }
            }
        }
    }
}
