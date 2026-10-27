#ifndef CAMERA_H
#define CAMERA_H

#include "vector_util.h"
#include <SFML/System.hpp>

class Camera {
public:
    Camera(const sf::Vector2f& windowSize, const sf::Vector2f& viewCenterWorld,
           float worldUnitsPerPixel = 1.f)
        : mWindowSize{windowSize},
          mViewCenterWorld{viewCenterWorld},
          mWorldUnitsPerPixel{worldUnitsPerPixel} {}

    // TODO: all of these:

    sf::Vector2f windowToWorld(const sf::Vector2f& pWindow) { 
        sf::Vector2f windowCenter = mWindowSize/2.0f;
        // Pword = C + (Pwindow - W/2)s
        return mViewCenterWorld + (pWindow - windowCenter) * mWorldUnitsPerPixel; };

    sf::Vector2f worldToWindow(const sf::Vector2f& pWorld) { 
        sf::Vector2f windowCenter = mWindowSize/2.0f;
        // Pwindow = W/2 + (Pworld-C)/s
        return windowCenter + (pWorld - mViewCenterWorld) / mWorldUnitsPerPixel; };

    sf::Vector2f windowPercentToWorld(const sf::Vector2f& pWindowPercent) { 
        // Calculate the point in the window
        sf::Vector2f pWindow = pWindowPercent * mWindowSize;
        // Convert to world coordinates 
        return windowToWorld(pWindow); };

    sf::Vector2f worldToWindowPercent(const sf::Vector2f& pWorld) { 
        // Convert to window coordinates
        sf::Vector2f pWindow = worldToWindow(pWorld);
        // Normalize the point 
        return pWindow / mWindowSize; };

    void translateWorld(const sf::Vector2f& deltaWorld) {
        // Translates the camera by deltaWorld world unit
        mViewCenterWorld += deltaWorld;
    };

    void translateWindow(const sf::Vector2f& deltaScreen) {
        // Translates the camera by deltaScreen window units.
        // Multiply the displacement by mWorldUnitsPerPixel -> world
        sf::Vector2f deltaWorld = deltaScreen * mWorldUnitsPerPixel;
        mViewCenterWorld += deltaWorld;
    };

    void translateWindowPercent(const sf::Vector2f& deltaScreenPercent) {
        // Translates the camera by deltaScreenPercent normalized window units
        // Multiply with the window size to get the displacement in screen
        sf::Vector2f deltaScreen = deltaScreenPercent * mWindowSize;
        translateWindow(deltaScreen);
    };

    void setCamera(const sf::Vector2f& pWorld, const sf::Vector2f& pWindow) {
        // maintains the camera's scale and translates the camera such that 
        // the given screen point now maps to the given world point
        sf::Vector2f windowCenter = mWindowSize / 2.0f;
        mViewCenterWorld = pWorld - (pWindow - windowCenter) * mWorldUnitsPerPixel;
    };

    void setCameraPercent(const sf::Vector2f& pWorld, const sf::Vector2f& pWindowPercent) {
        // takes a screen point given as a percentage of the total window size from the top-left
        // Calculate the point in window
        sf::Vector2f pWindow = pWindowPercent * mWindowSize;
        setCamera(pWorld, pWindow);
    };

    void setCameraCenter(const sf::Vector2f& pWorld) {
        mViewCenterWorld = pWorld;
    };

    sf::Vector2f getCameraCenter() const { 
        return mViewCenterWorld;
    };

private:
    sf::Vector2f mWindowSize;       // size of the window in screen pixels (i.e. window coords)
    sf::Vector2f mViewCenterWorld;  // world coords point corresponding to the center of our window.
    float mWorldUnitsPerPixel;      // distance in world coords covered by one pixel
};

#endif  // CAMERA_H
