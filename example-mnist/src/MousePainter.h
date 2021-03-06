/*
 * Simple class to implement drawing with mouse
 *
 */

#pragma once

#include "ofMain.h"

class MousePainter {
public:
    ofColor paint_color = ofColor::white;
    ofColor erase_color = ofColor::black;
    float radius = 7;

    void setup(int size = 256) {
        fbo.allocate(size, size);
        clear();
    }

    void clear() {
        fbo.begin();
        ofClear(0, 0, 0, 255);
        fbo.end();
    }

    void penDown(ofVec2f p, bool do_erase = false) {
        ofPushStyle();

        ofSetColor(do_erase ? erase_color : paint_color);
        float r = do_erase ? 2 * radius : radius;   // if erasing, using double radius

        fbo.begin();
        ofDrawCircle(p, r);
        fbo.end();

        ofPopStyle();
        last_point = p; // save last point
    }

    void penDrag(ofVec2f p, bool do_erase = false) {
        ofPushStyle();
        ofSetColor(do_erase ? erase_color : paint_color);
        float r = do_erase ? 2 * radius : radius;   // if erasing, using double radius

        fbo.begin();
        // lines can't be wide enough, so painting with a rotated rectangle, innit
        ofPushMatrix();
        ofTranslate(last_point);
        ofVec2f diff(p - last_point);
        ofRotate(ofRadToDeg(atan2(diff.y, diff.x)));
        ofDrawRectangle(0, -r, diff.length(), 2 * r);
        ofPopMatrix();

        fbo.end();
        ofPopStyle();

        // also draw a circle to cover up the ugly joins
        penDown(p, do_erase);
    }

    void penUp() {

    }

    void draw() {
        ofPushStyle();
        ofSetColor(255);
        fbo.draw(0, 0);
        ofNoFill();
        ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
        ofPopStyle();
    }

    const ofPixels& get_pixels() {
        fbo.readToPixels(pixels);
        return pixels;
    }

    int getWidth() { return fbo.getWidth(); }
    int getHeight() { return fbo.getHeight(); }

protected:
    ofVec2f last_point;
    ofFbo fbo;
    ofPixels pixels;
};
