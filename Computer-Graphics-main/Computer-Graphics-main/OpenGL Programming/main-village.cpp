// ============================================================================
// REALISTIC VILLAGE SCENARIO WITH RIVER BOAT (FINAL IMPROVED v4)
// Advanced Computer Graphics Project - 2D Village with Realistic Elements
// ============================================================================

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Window dimensions
const int WIDTH  = 1400;
const int HEIGHT = 800;

// Animation states
float sunAngle        = 0.0f;   // now used as full day-night cycle 0..2PI
float cloudOffset     = 0.0f;
float boatPosition    = 0.0f;
float birdOffset      = 0.0f;
float windmillAngle = 0.0f;
float carPosition     = 0.0f;
bool  isDay = true;
bool  animationPaused = false;

// Extra animation states
float swingAngle      = 0.0f;
bool  swingForward    = true;
float planePosition   = 0.0f;
bool  isRaining       = false;
float rainOffset      = 0.0f;
float speedFactor     = 1.0f;

// ENHANCED ELEMENTS
float trainPosition   = WIDTH + 400.0f;  // Start from right side
float personPosition = 0.0f;
// Multiple walkers on road
const int NUM_WALKERS = 5;
float walkerPositions[NUM_WALKERS];
float walkerSpeeds[NUM_WALKERS] = {0.8f, 1.2f, 0.9f, 1.5f, 1.1f};
float riverWave = 0.0f;
float fishPosition    = -300.0f;         // fish starts 300px left
float smokeOffset     = 0.0f;
float dayNightBlend   = 1.0f;
float sunGlow         = 0.0f;
float windIntensity   = 1.0f;
float windUser = 1.0f;   // user wind multiplier (W/S changes this)
int trainBogieCount = 5; 


// NEW: extra animations
float balloonPosition = 0.0f;    // hot air balloon
float kitePosition    = 0.0f;    // kite in the sky

// TOGGLE FLAGS (for viva/demo)
bool showBirds        = true;
bool showPlane        = true;
bool showTrain        = true;
bool showPerson       = true;



bool useScaleT   = false;
bool useRotateT  = false;
bool useReflectT = false;
bool useShearT   = false;



// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

void initRendering();

// Drawing utilities
void drawCircle(float cx, float cy, float r, int segments = 40);
void drawEllipse(float cx, float cy, float rx, float ry, int segments = 40);
void drawShadowEllipse(float cx, float cy, float rx, float ry, float alpha);


// --- ALGORITHM DRAWING (for teacher requirement) ---
void drawLineDDA(int x1, int y1, int x2, int y2);
void drawLineBresenham(int x1, int y1, int x2, int y2);
void drawCircleMidpoint(int cx, int cy, int r);


// Background elements
void drawSky();
void drawSunMoon();
void drawClouds();
void drawDistantHills();
void drawStars();

// Terrain and landscape
void drawGround();
void drawRiver();
void drawRailTrack();
void drawRoad();

// Houses
void drawModernHouse(float x, float y);
void drawTraditionalHouse(float x, float y);
void drawFarmHouse(float x, float y);
void drawCottage(float x, float y);

// Vegetation
void drawTree(float x, float y, int type = 0);
void drawBush(float x, float y, float size);

// Vehicles
void drawTrain(float x, float y);
void drawCar(float x, float y);
void drawBoat(float x, float y);
void drawBoatWaterReflection(float boatX, float boatY);
void drawAirplane();
void drawFish();

// Animated objects
void drawMovingTrain();
void drawMovingCar();
void drawMovingBoat();
void drawBirds();
void drawWalkingPerson();

// Structures
void drawWindmill(float x, float y);

// NEW EXTRAS
void drawSwing(float x, float y);        // kept – used as seesaw angle source
void drawDockAndFisherman();            // river ghat + fishing man
void drawFieldAndCow();                 // crop field + cows (+ fence)
void drawHotAirBalloon();               // sky balloon
void drawFireflies();                   // night fireflies
void drawKite();                        // kite in sky
// void drawWelcomeSign();                 // "WELCOME TO VILLAGE" board
void drawWell(float x, float y);        // small well near house

// Cow helper
void drawSingleCow(float cx, float cy, float scale, bool faceRight);

// Weather effects
void drawRain();

// Scene composition
void drawVillageScene();

// Callbacks
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void reshape(int w, int h);

// ============================================================================
// INIT - SMALL QUALITY IMPROVEMENT (SMOOTHER LINES/POINTS)
// ============================================================================

void initRendering() {
    glClearColor(0.1f, 0.15f, 0.25f, 1.0f);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

// ============================================================================
// DRAWING UTILITIES
// ============================================================================

void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = cx + r * std::cos(angle);
        float y = cy + r * std::sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = cx + rx * std::cos(angle);
        float y = cy + ry * std::sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawShadowEllipse(float cx, float cy, float rx, float ry, float alpha) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);
    drawEllipse(cx, cy, rx, ry, 32);
    glDisable(GL_BLEND);
}


static inline void plotPoint(int x, int y) {
    glVertex2i(x, y);
}

void drawLineDDA(int x1, int y1, int x2, int y2) {
    float xdif = (float)x2 - (float)x1;
    float ydif = (float)y2 - (float)y1;

    int steps = (fabs(xdif) > fabs(ydif)) ? (int)fabs(xdif) : (int)fabs(ydif);

    glBegin(GL_POINTS);

    if (steps == 0) {
        plotPoint(x1, y1);
        glEnd();
        return;
    }

    float xinc = xdif / steps;
    float yinc = ydif / steps;

    float x = (float)x1, y = (float)y1;
    for (int i = 0; i <= steps; i++) {
        plotPoint((int)round(x), (int)round(y));
        x += xinc; y += yinc;
    }

    glEnd();
}



void drawLineBresenham(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        int ys = (y1 < y2) ? y1 : y2;
        int ye = (y1 < y2) ? y2 : y1;
        glBegin(GL_POINTS);
        for (int y = ys; y <= ye; y++) plotPoint(x1, y);
        glEnd();
        return;
    }
    float m = float(y2 - y1) / float(x2 - x1);

    if (fabs(m) <= 1.0f && x1 > x2) { std::swap(x1, x2); std::swap(y1, y2); }
    if (m > 1.0f && y1 > y2)        { std::swap(x1, x2); std::swap(y1, y2); }
    if (m < -1.0f && y1 < y2)       { std::swap(x1, x2); std::swap(y1, y2); }

    // recompute after swap
    int dx = x2 - x1;
    int dy = y2 - y1;
    m = float(dy) / float(dx);

    int x, y, pk;

    // SAME CASES 
    if (m >= 0 && m <= 1) {
        pk = (2 * (y2 - y1) - (x2 - x1));
        x = x1; y = y1;

        glBegin(GL_POINTS);
        for (int i = 0; i <= (x2 - x1); i++) {
            plotPoint(x, y);
            if (pk < 0) {
                x = x + 1;
                pk = pk + (2 * (y2 - y1));
            } else {
                x = x + 1;
                y = y + 1;
                pk = pk + (2 * (y2 - y1)) - (2 * (x2 - x1));
            }
        }
        glEnd();
    }
    else if (m > 1) {
        pk = (2 * (x2 - x1) - (y2 - y1));
        x = x1; y = y1;

        glBegin(GL_POINTS);
        for (int i = 0; i <= (y2 - y1); i++) {
            plotPoint(x, y);
            if (pk < 0) {
                y = y + 1;
                pk = pk + (2 * (x2 - x1));
            } else {
                x = x + 1;
                y = y + 1;
                pk = pk + (2 * (x2 - x1)) - (2 * (y2 - y1));
            }
        }
        glEnd();
    }
    else if (m < 0 && m >= -1) {
        pk = (2 * (y1 - y2) - (x2 - x1));
        x = x1; y = y1;

        glBegin(GL_POINTS);
        for (int i = 0; i <= (x2 - x1); i++) {
            plotPoint(x, y);
            if (pk < 0) {
                x = x + 1;
                pk = pk + (2 * (y1 - y2));
            } else {
                x = x + 1;
                y = y - 1;
                pk = pk + (2 * (y1 - y2)) - (2 * (x2 - x1));
            }
        }
        glEnd();
    }
    else { // m < -1
        pk = (2 * (x2 - x1) - (y1 - y2));
        x = x1; y = y1;

        glBegin(GL_POINTS);
        for (int i = 0; i <= (y1 - y2); i++) {
            plotPoint(x, y);
            if (pk < 0) {
                y = y - 1;
                pk = pk + (2 * (x2 - x1));
            } else {
                x = x + 1;
                y = y - 1;
                pk = pk + (2 * (x2 - x1)) - (2 * (y1 - y2));
            }
        }
        glEnd();
    }
}



void drawCircleMidpoint(int cx, int cy, int r) {
    int x = 0;
    int y = r;
    int p = 1 - r;

    auto plot8 = [&](int px, int py) {
        plotPoint(cx + px, cy + py);
        plotPoint(cx + py, cy + px);

        plotPoint(cx - px, cy + py);
        plotPoint(cx - py, cy + px);

        plotPoint(cx - px, cy - py);
        plotPoint(cx - py, cy - px);

        plotPoint(cx + px, cy - py);
        plotPoint(cx + py, cy - px);
    };

    glBegin(GL_POINTS);

    while (x < y) {
        plot8(x, y);

        if (p < 0) {
            x = x + 1;
            p = p + (2 * x) + 3;
        } else {
            x = x + 1;
            y = y - 1;
            p = p + (2 * x) - (2 * y) + 5;
        }
    }

    glEnd();
}

struct Vec2 {
    float x, y;
};

// ---------- Basic 2D Transformation Algorithms (NO OpenGL built-in) ----------
static inline Vec2 translate2D(Vec2 p, float tx, float ty) {
    p.x += tx;
    p.y += ty;
    return p;
}

static inline Vec2 scale2D(Vec2 p, float sx, float sy) {
    p.x *= sx;
    p.y *= sy;
    return p;
}

static inline Vec2 rotate2D(Vec2 p, float angle) {
    float rad = angle * (M_PI / 180.0f);
    float newX = (p.x * cos(rad)) - (p.y * sin(rad));
    float newY = (p.x * sin(rad)) + (p.y * cos(rad));
    p.x = newX;
    p.y = newY;
    return p;
}



// Reflection by algorithm
static inline Vec2 reflectX(Vec2 p) { p.y = -p.y; return p; } // about X-axis
static inline Vec2 reflectY(Vec2 p) { p.x = -p.x; return p; } // about Y-axis

// Shear by algorithm
// x' = x + shx*y
// y' = y + shy*x
static inline Vec2 shear2D(Vec2 p, float shx, float shy) {
    return { p.x + shx * p.y, p.y + shy * p.x };
}

// Apply all transforms in order (algorithm pipeline)
static inline Vec2 applyAllTransforms(Vec2 p,
                                     float sx, float sy,
                                     float rotDeg,
                                     bool doReflectX, bool doReflectY,
                                     float shx, float shy,
                                     float tx, float ty)
{
    // 1) Scale
    p = scale2D(p, sx, sy);

    // 2) Rotate
    p = rotate2D(p, rotDeg);

    // 3) Reflect
    if (doReflectX) p = reflectX(p);
    if (doReflectY) p = reflectY(p);

    // 4) Shear
    p = shear2D(p, shx, shy);

    // 5) Translate (final)
    p = translate2D(p, tx, ty);

    return p;
}



// ============================================================================
// SKY AND BACKGROUND WITH SMOOTH TRANSITIONS
// ============================================================================

void drawSky() {
    if (isDay && dayNightBlend < 1.0f) dayNightBlend += 0.02f;
    if (!isDay && dayNightBlend > 0.0f) dayNightBlend -= 0.02f;

    float dayFactor   = dayNightBlend;
    float nightFactor = 1.0f - dayFactor;

    // Day colors
    float dayTopR = 0.5f, dayTopG = 0.7f, dayTopB = 1.0f;
    float dayBotR = 0.7f, dayBotG = 0.85f, dayBotB = 1.0f;

    // Night colors
    float nightTopR = 0.05f, nightTopG = 0.05f, nightTopB = 0.2f;
    float nightBotR = 0.1f,  nightBotG = 0.1f,  nightBotB = 0.3f;

    glBegin(GL_QUADS);
    glColor3f(dayTopR * dayFactor + nightTopR * nightFactor,
              dayTopG * dayFactor + nightTopG * nightFactor,
              dayTopB * dayFactor + nightTopB * nightFactor);
    glVertex2f(0, HEIGHT);
    glVertex2f(WIDTH, HEIGHT);

    glColor3f(dayBotR * dayFactor + nightBotR * nightFactor,
              dayBotG * dayFactor + nightBotG * nightFactor,
              dayBotB * dayFactor + nightBotB * nightFactor);
    glVertex2f(WIDTH, HEIGHT * 0.6f);
    glVertex2f(0, HEIGHT * 0.6f);
    glEnd();
}

void drawStars() {
    if (isDay || dayNightBlend > 0.3f) return;

    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) {
        float x = (i * 37) % WIDTH;
        float y = HEIGHT * 0.7f + (i * 53) % (int)(HEIGHT * 0.3f);
        if (i % 3 == 0) glColor3f(1.0f, 1.0f, 1.0f);
        else if (i % 3 == 1) glColor3f(0.9f, 0.9f, 1.0f);
        else glColor3f(1.0f, 0.9f, 0.9f);
        glVertex2f(x, y);
    }
    glEnd();
}

// NEW: sun & moon move from horizon (bottom of sky) up and again down
// ======================= REPLACE THIS FUNCTION =======================
void drawSunMoon() {
    const float PI = 3.1415926f;

    float t = std::fmod(sunAngle, 2.0f * PI);
    if (t < 0.0f) t += 2.0f * PI;

    float horizonY  = 460.0f;
    float sunAmp    = 130.0f;
    float moonAmp   = 100.0f;

    // enable blending so glColor4f alpha actually works (glow, rays)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (t < PI) {
        // ---------------------- SUN (DAY) ----------------------------------
        float local = t;
        float phase = local / PI;

        float x = WIDTH * 0.15f + (WIDTH * 0.70f) * phase;
        float y = horizonY + std::sin(local) * sunAmp;

        sunGlow = 0.3f + 0.2f * std::sin(local * 3.0f);

        // glow layers
        for (int i = 3; i > 0; --i) {
            float alpha = 0.10f * i;
            glColor4f(1.0f, 1.0f, 0.5f, alpha);
            drawCircle(x, y, 30.0f + i * 10.0f);
        }

        // main sun
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        drawCircle(x, y, 30.0f);

        // rays
        glColor4f(1.0f, 1.0f, 0.5f, 0.55f);
        glLineWidth(2.5f + sunGlow);
        for (int i = 0; i < 16; i++) {
            float ang = 2.0f * PI * i / 16.0f + local * 0.5f;
            float x1 = x + std::cos(ang) * 40.0f;
            float y1 = y + std::sin(ang) * 40.0f;
            float x2 = x + std::cos(ang) * 60.0f;
            float y2 = y + std::sin(ang) * 60.0f;

            glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glEnd();
        }
    } else {
        // ---------------------- MOON (NIGHT) -------------------------------
        float local = t - PI;
        float phase = local / PI;

        float x = WIDTH * 0.15f + (WIDTH * 0.70f) * phase;
        float y = horizonY + std::sin(local) * moonAmp + 10.0f;

        float bright = 0.8f + 0.2f * std::sin(local * 0.8f);

        glColor4f(0.9f * bright, 0.9f * bright, 0.9f * bright, 1.0f);
        drawCircle(x, y, 28.0f);

        glColor4f(0.7f * bright, 0.7f * bright, 0.7f * bright, 1.0f);
        drawCircle(x - 10.0f, y + 10.0f, 6.0f);
        drawCircle(x + 12.0f, y -  8.0f, 5.0f);
        drawCircle(x +  8.0f, y + 12.0f, 4.0f);

        // moon glow
        glColor4f(0.9f, 0.9f, 1.0f, 0.20f);
        drawCircle(x, y, 35.0f);
    }

    glDisable(GL_BLEND);
}


void drawClouds() {
    
    float speed = 0.5f * windIntensity;

    for (int layer = 0; layer < 3; layer++) {
        float layerSpeed = speed * (layer + 1) * 0.5f;
        float layerY = 500.0f + layer * 40.0f;
        float layerAlpha = 0.7f - layer * 0.2f;

        glColor4f(1.0f, 1.0f, 1.0f, layerAlpha);

        float cloud1X = 150.0f + cloudOffset * layerSpeed;
        drawCircle(cloud1X, layerY, 25.0f);
        drawCircle(cloud1X + 22.0f, layerY + 12.0f, 22.0f);
        drawCircle(cloud1X + 45.0f, layerY, 25.0f);
        drawCircle(cloud1X + 22.0f, layerY - 10.0f, 20.0f);

        // ✅ changed direction: now moves SAME way (right)
        float cloud2X = 600.0f + cloudOffset * layerSpeed * 0.8f;
        drawCircle(cloud2X, layerY + 20.0f, 30.0f);
        drawCircle(cloud2X + 28.0f, layerY + 30.0f, 25.0f);
        drawCircle(cloud2X + 55.0f, layerY + 20.0f, 28.0f);

        float cloud3X = 1000.0f + cloudOffset * layerSpeed * 0.6f;
        drawCircle(cloud3X, layerY - 10.0f, 20.0f);
        drawCircle(cloud3X + 20.0f, layerY, 18.0f);
        drawCircle(cloud3X + 38.0f, layerY - 10.0f, 22.0f);
    }
}




void drawDistantHills() {

    auto drawHillLayer = [&](float baseY, float bottomY,
                             float amp1, float f1,
                             float amp2, float f2,
                             float step,
                             float r, float g, float b)
    {
        glColor3f(r, g, b);
        glBegin(GL_POLYGON);

        glVertex2f(0, bottomY);
        glVertex2f(0, baseY);

        for (float x = 0; x <= WIDTH; x += step) {
            // mix 2 sine waves -> irregular, more natural
            float h = baseY
                    + amp1 * std::sin(x * f1)
                    + amp2 * std::sin(x * f2 + 1.7f);
            glVertex2f(x, h);
        }

        glVertex2f(WIDTH, baseY);
        glVertex2f(WIDTH, bottomY);

        glEnd();
    };

    // ---------------- FAR layer (light, smooth) ----------------
    drawHillLayer(
        465.0f, 400.0f,
        35.0f, 0.0065f,
        18.0f, 0.0140f,
        25.0f,
        0.10f, 0.38f, 0.10f
    );

    // ---------------- MID layer ----------------
    drawHillLayer(
        445.0f, 380.0f,
        45.0f, 0.0080f,
        22.0f, 0.0180f,
        22.0f,
        0.08f, 0.34f, 0.08f
    );

    // ---------------- NEAR layer (darker, more detail) ----------------
    drawHillLayer(
        425.0f, 360.0f,
        55.0f, 0.0100f,
        28.0f, 0.0240f,
        18.0f,
        0.06f, 0.28f, 0.06f
    );

    // ---------------- VERY NEAR strip (small bumps, foreground feel) ----------------
    drawHillLayer(
        405.0f, 350.0f,
        28.0f, 0.0180f,
        14.0f, 0.0450f,
        14.0f,
        0.05f, 0.22f, 0.05f
    );

    //  Optional: light mist line to soften horizon (looks very realistic)
    if (!isDay) return; // keep mist mainly daytime (you can remove this line)

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.9f, 0.95f, 1.0f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(0, 455);
        glVertex2f(WIDTH, 455);
        glVertex2f(WIDTH, 395);
        glVertex2f(0, 395);
    glEnd();
    glDisable(GL_BLEND);
}


void drawGround() {
    float f = dayNightBlend;

    float dayLowR = 0.25f, dayLowG = 0.55f, dayLowB = 0.25f;
    float dayHighR = 0.35f, dayHighG = 0.65f, dayHighB = 0.35f;

    float nightLowR = 0.07f, nightLowG = 0.18f, nightLowB = 0.07f;
    float nightHighR = 0.12f, nightHighG = 0.25f, nightHighB = 0.12f;

    float lowR  = dayLowR  * f + nightLowR  * (1.0f - f);
    float lowG  = dayLowG  * f + nightLowG  * (1.0f - f);
    float lowB  = dayLowB  * f + nightLowB  * (1.0f - f);
    float highR = dayHighR * f + nightHighR * (1.0f - f);
    float highG = dayHighG * f + nightHighG * (1.0f - f);
    float highB = dayHighB * f + nightHighB * (1.0f - f);

    glBegin(GL_QUADS);
    glColor3f(lowR, lowG, lowB);
    glVertex2f(0, 0);
    glVertex2f(WIDTH, 0);
    glColor3f(highR, highG, highB);
    glVertex2f(WIDTH, 380);
    glVertex2f(0, 380);
    glEnd();
}

void drawRoad() {
    float f = dayNightBlend;

    // ── Road base colors (slightly darker at edges for compacted-soil look) ──
    float dayR_mid = 0.62f, dayG_mid = 0.49f, dayB_mid = 0.34f;
    float dayR_edge = 0.55f, dayG_edge = 0.44f, dayB_edge = 0.27f;
    float nightR_mid = 0.31f, nightG_mid = 0.24f, nightB_mid = 0.17f;
    float nightR_edge = 0.27f, nightG_edge = 0.21f, nightB_edge = 0.13f;

    float rMid  = dayR_mid  * f + nightR_mid  * (1.0f - f);
    float gMid  = dayG_mid  * f + nightG_mid  * (1.0f - f);
    float bMid  = dayB_mid  * f + nightB_mid  * (1.0f - f);
    float rEdge = dayR_edge * f + nightR_edge * (1.0f - f);
    float gEdge = dayG_edge * f + nightG_edge * (1.0f - f);
    float bEdge = dayB_edge * f + nightB_edge * (1.0f - f);

    // ── Road body (y=190 to y=280) with gradient: darker top/bottom edges ──
    glBegin(GL_QUADS);
    glColor3f(rEdge, gEdge, bEdge); glVertex2f(0,     190);
    glColor3f(rEdge, gEdge, bEdge); glVertex2f(WIDTH, 190);
    glColor3f(rMid,  gMid,  bMid);  glVertex2f(WIDTH, 235);
    glColor3f(rMid,  gMid,  bMid);  glVertex2f(0,     235);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(rMid,  gMid,  bMid);  glVertex2f(0,     235);
    glColor3f(rMid,  gMid,  bMid);  glVertex2f(WIDTH, 235);
    glColor3f(rEdge, gEdge, bEdge); glVertex2f(WIDTH, 280);
    glColor3f(rEdge, gEdge, bEdge); glVertex2f(0,     280);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ── Surface erosion / horizontal crack lines ──
    glColor4f(0.35f, 0.22f, 0.10f, 0.40f);
    glLineWidth(0.8f);
    struct CrackSeg { float x1, y1, x2, y2, x3, y3; };
    CrackSeg cracks[] = {
        {20,  225, 55,  221, 90,  226},
        {180, 220, 215, 215, 250, 221},
        {380, 222, 415, 218, 455, 223},
        {555, 220, 580, 215, 612, 220},
    };
    for (auto& c : cracks) {
        glBegin(GL_LINE_STRIP);
        glVertex2f(c.x1, c.y1);
        glVertex2f(c.x2, c.y2);
        glVertex2f(c.x3, c.y3);
        glEnd();
    }

    // ── Scattered gravel dots ──
    auto drawGravel = [](float cx, float cy, float rx, float ry) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 12; i++) {
            float a = i * 3.14159f / 6.0f;
            glVertex2f(cx + cos(a) * rx, cy + sin(a) * ry);
        }
        glEnd();
    };

    // Top-edge gravel
    glColor4f(0.42f, 0.30f, 0.16f, 0.55f);
    float gravelTop[][2] = {
        {28,193},{72,196},{115,192},{160,195},{210,193},
        {258,196},{300,192},{345,194},{390,196},{440,193},
        {490,195},{535,192},{580,194},{630,196},{668,193}
    };
    for (auto& g : gravelTop) drawGravel(g[0], g[1], 3.0f, 1.8f);

    // Bottom-edge gravel
    float gravelBot[][2] = {
        {40,277},{90,274},{140,278},{195,274},{255,277},
        {310,275},{370,278},{425,275},{480,277},{540,274},
        {600,278},{650,275}
    };
    for (auto& g : gravelBot) drawGravel(g[0], g[1], 3.0f, 1.8f);

    // Mid-road lighter grit
    glColor4f(0.55f, 0.40f, 0.22f, 0.30f);
    float gravelMid[][2] = {
        {55,234},{130,228},{220,232},{330,229},
        {410,233},{510,230},{620,234}
    };
    for (auto& g : gravelMid) drawGravel(g[0], g[1], 2.5f, 1.5f);

    // ─────────────────────────────────────────────────────────
    // POTHOLES — organic polygon shapes
    // Each has: shadow rim, irregular pit, optional puddle,
    // crumbled-edge chunks, and depth shading
    // ─────────────────────────────────────────────────────────
    auto drawPothole = [](float cx, float cy, float rx, float ry,
                          const float offsets[8], bool hasPuddle,
                          float nightBlend)
    {
        const int segs = 64;
        const int ctrl = 8;

        auto getRadius = [&](float angle) -> float {
            float t = angle / (2.0f * 3.14159f) * ctrl;
            int   i = (int)t % ctrl;
            int   j = (i + 1) % ctrl;
            float s = t - (int)t;
            s = s * s * (3.0f - 2.0f * s); // smooth-step
            return 1.0f + offsets[i] * (1.0f - s) + offsets[j] * s;
        };

        // 1. Shadow rim
        glColor4f(0.10f, 0.06f, 0.02f, 0.55f);
        glBegin(GL_POLYGON);
        for (int i = 0; i < segs; i++) {
            float a = i * 2.0f * 3.14159f / segs;
            float r = getRadius(a);
            glVertex2f(cx + cos(a) * rx * r * 1.18f,
                       cy + sin(a) * ry * r * 1.18f);
        }
        glEnd();

        // 2. Pit body
        float pitR = 0.24f * nightBlend + 0.12f * (1.0f - nightBlend);
        float pitG = 0.14f * nightBlend + 0.07f * (1.0f - nightBlend);
        float pitB = 0.06f * nightBlend + 0.03f * (1.0f - nightBlend);
        glColor3f(pitR, pitG, pitB);
        glBegin(GL_POLYGON);
        for (int i = 0; i < segs; i++) {
            float a = i * 2.0f * 3.14159f / segs;
            float r = getRadius(a);
            glVertex2f(cx + cos(a) * rx * r,
                       cy + sin(a) * ry * r);
        }
        glEnd();

        // 3. Inner depth gradient (darker center)
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(pitR * 0.55f, pitG * 0.55f, pitB * 0.55f);
        glVertex2f(cx, cy);
        glColor3f(pitR, pitG, pitB);
        for (int i = 0; i <= segs; i++) {
            float a = i * 2.0f * 3.14159f / segs;
            float r = getRadius(a);
            glVertex2f(cx + cos(a) * rx * r,
                       cy + sin(a) * ry * r);
        }
        glEnd();

        // 4. Optional puddle
        if (hasPuddle) {
            float pRx = rx * 0.55f, pRy = ry * 0.52f;
            glColor4f(0.38f, 0.52f, 0.62f, 0.72f);
            glBegin(GL_POLYGON);
            for (int i = 0; i < segs; i++) {
                float a = i * 2.0f * 3.14159f / segs;
                glVertex2f(cx + cos(a) * pRx, cy + sin(a) * pRy);
            }
            glEnd();
            // sky highlight streak
            glColor4f(1.0f, 1.0f, 1.0f, 0.14f);
            glBegin(GL_POLYGON);
            for (int i = 0; i < segs; i++) {
                float a = i * 2.0f * 3.14159f / segs;
                glVertex2f(cx - pRx * 0.30f + cos(a) * pRx * 0.38f,
                           cy - pRy * 0.25f + sin(a) * pRy * 0.32f);
            }
            glEnd();
        }

        // 5. Crumbled edge chunks
        glColor4f(0.42f, 0.27f, 0.12f, 0.72f);
        float chunkAngles[] = {3.14159f * 1.1f, 3.14159f * 0.1f, 3.14159f * 0.6f};
        for (float ca : chunkAngles) {
            float r = getRadius(ca);
            float bx = cx + cos(ca) * rx * r;
            float by = cy + sin(ca) * ry * r;
            glBegin(GL_POLYGON);
            for (int i = 0; i < 8; i++) {
                float a = i * 2.0f * 3.14159f / 8;
                glVertex2f(bx + cos(a) * 4.5f, by + sin(a) * 2.5f);
            }
            glEnd();
        }

        // 6. Dark outline for rim depth
        glColor4f(0.15f, 0.08f, 0.03f, 0.55f);
        glLineWidth(1.8f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segs; i++) {
            float a = i * 2.0f * 3.14159f / segs;
            float r = getRadius(a);
            glVertex2f(cx + cos(a) * rx * r, cy + sin(a) * ry * r);
        }
        glEnd();
    };

    // Pothole definitions: {cx, cy, rx, ry, hasPuddle, offsets[8]}
    struct PHDef {
        float cx, cy, rx, ry;
        bool  puddle;
        float off[8];
    };
    PHDef holes[] = {
        {150,  240, 22, 14, true,  {0.10f,-0.15f, 0.08f,-0.12f, 0.15f,-0.08f, 0.12f,-0.10f}},
        {400,  225, 20, 12, false, {-0.12f, 0.10f,-0.08f, 0.14f,-0.10f, 0.12f,-0.06f, 0.09f}},
        {650,  255, 25, 16, true,  {0.08f,-0.10f, 0.14f,-0.12f, 0.06f,-0.14f, 0.10f,-0.08f}},
        {850,  235, 18, 11, false, {-0.08f, 0.12f,-0.14f, 0.08f,-0.10f, 0.06f,-0.12f, 0.10f}},
        {1050, 250, 28, 17, true,  {0.12f,-0.08f, 0.16f,-0.10f, 0.08f,-0.14f, 0.10f,-0.12f}},
    };

    for (auto& h : holes)
        drawPothole(h.cx, h.cy, h.rx, h.ry, h.off, h.puddle, f);

    // ── Mud splash marks around puddled potholes ──
    glColor4f(0.40f, 0.26f, 0.10f, 0.32f);
    struct Splash { float cx, cy, rx, ry, angle; };
    Splash splashes[] = {
        {122, 227, 5, 2, -20}, {112, 250, 4, 2,  35}, {165, 255, 4.5f, 2,  0},
        {616, 241, 5, 2,  15}, {638, 268, 4, 2, -30}, {670, 260, 5,   2, 20},
        {1012,235, 5, 2, -15}, {1022,262, 4, 2,  30}, {1070,260, 5,   2, 10},
    };
    for (auto& s : splashes) {
        glPushMatrix();
        glTranslatef(s.cx, s.cy, 0);
        glRotatef(s.angle, 0, 0, 1);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 10; i++) {
            float a = i * 2.0f * 3.14159f / 10;
            glVertex2f(cos(a) * s.rx, sin(a) * s.ry);
        }
        glEnd();
        glPopMatrix();
    }

    glDisable(GL_BLEND);
    glLineWidth(1.0f);
}

void drawRiver() {
    float topY    = 180.0f;
    float bottomY = 120.0f;
    float midY    = (topY + bottomY) * 0.5f;

    float waveIntensity = 3.0f + 2.0f * std::sin(riverWave * 0.05f);

    // helper: compute water colors (top & bottom) for any x
    auto getColors = [&](float x,
                         float &topR, float &topG, float &topB,
                         float &botR, float &botG, float &botB)
    {
        // base day/night
        float dayR = 0.18f, dayG = 0.42f, dayB = 0.85f;
        float nR   = 0.08f, nG   = 0.20f, nB   = 0.45f;

        float baseR = dayR * dayNightBlend + nR * (1.0f - dayNightBlend);
        float baseG = dayG * dayNightBlend + nG * (1.0f - dayNightBlend);
        float baseB = dayB * dayNightBlend + nB * (1.0f - dayNightBlend);

        // depth: middle darker + slight variation
        float depth = 0.10f + 0.10f * std::sin(x * 0.02f + riverWave * 0.02f);

        topR = baseR + 0.10f;
        topG = baseG + 0.10f;
        topB = baseB + 0.12f;

        botR = baseR - depth;
        botG = baseG - depth * 0.9f;
        botB = baseB - depth * 0.6f;

        // daytime sparkle zone
        if (isDay && x > WIDTH * 0.70f && x < WIDTH * 0.92f) {
            float s = 0.10f + 0.10f * std::sin((x - WIDTH * 0.78f) * 0.06f + riverWave * 0.10f);
            topR += s; topG += s; topB += s;
        }
    };

    // -------------------- WATER BODY (quad strip) --------------------
    glBegin(GL_QUAD_STRIP);

    for (int xi = 0; xi < WIDTH; xi += 18) {   // ✅ < WIDTH
        float x = (float)xi;

        float w1 = waveIntensity * std::sin(x * 0.03f + riverWave * 0.10f);
        float w2 = waveIntensity * std::sin(x * 0.03f + riverWave * 0.10f + 0.55f);

        float topR, topG, topB, botR, botG, botB;
        getColors(x, topR, topG, topB, botR, botG, botB);

        glColor3f(topR, topG, topB);
        glVertex2f(x, topY + w1);

        glColor3f(botR, botG, botB);
        glVertex2f(x, bottomY + w2);
    }

    // FORCE LAST VERTEX at WIDTH (fix right-side missing water)
    {
        float x = (float)WIDTH;

        float w1 = waveIntensity * std::sin(x * 0.03f + riverWave * 0.10f);
        float w2 = waveIntensity * std::sin(x * 0.03f + riverWave * 0.10f + 0.55f);

        float topR, topG, topB, botR, botG, botB;
        getColors(x, topR, topG, topB, botR, botG, botB);

        glColor3f(topR, topG, topB);
        glVertex2f(x, topY + w1);

        glColor3f(botR, botG, botB);
        glVertex2f(x, bottomY + w2);
    }

    glEnd();

    // -------------------- SPARKLES (daytime glitter) --------------------
    if (isDay) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPointSize(2.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < 120; i++) {
            float px = (i * 37 + (int)(riverWave * 10)) % WIDTH;
            float py = midY + std::sin((px * 0.03f) + riverWave * 0.08f + i) * 10.0f;

            float a = 0.10f + 0.25f * (0.5f + 0.5f * std::sin(riverWave * 0.15f + i));
            glColor4f(1.0f, 1.0f, 1.0f, a);
            glVertex2f(px, py);
        }
        glEnd();

        glDisable(GL_BLEND);
    }

    // -------------------- BANK FOAM --------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.90f, 0.95f, 1.0f, 0.18f);
    for (int x = 0; x <= WIDTH; x += 28) {
        float t = std::sin(x * 0.08f + riverWave * 0.18f) * 2.0f;
        drawEllipse((float)x, topY - 3.0f + t,    10.0f, 2.6f, 18);
        drawEllipse((float)x, bottomY + 3.0f + t, 10.0f, 2.6f, 18);
    }

    // -------------------- SUBTLE RIPPLE RINGS --------------------
    glColor4f(0.85f, 0.95f, 1.0f, 0.10f);
    for (int i = 0; i < 6; i++) {
        float rx = (200.0f + i * 170.0f) + std::sin(riverWave * 0.04f + i) * 25.0f;
        float ry = 140.0f + std::sin(riverWave * 0.07f + i * 2.0f) * 8.0f;
        float rr = 10.0f + std::fmod(riverWave * (1.2f + i * 0.25f), 18.0f);
        drawEllipse(rx, ry, rr, rr * 0.35f, 30);
    }

    glDisable(GL_BLEND);

    // -------------------- MOON REFLECTION (night) --------------------
    if (!isDay) {
        float mx = WIDTH * 0.85f;
        float cy = midY;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.9f, 0.9f, 1.0f, 0.22f);
        drawEllipse(mx, cy, 48.0f, 9.0f, 44);

        glColor4f(0.9f, 0.9f, 1.0f, 0.12f);
        drawEllipse(mx + 10.0f, cy - 7.0f, 36.0f, 7.0f, 36);
        drawEllipse(mx - 8.0f,  cy - 13.0f, 28.0f, 6.0f, 32);

        glDisable(GL_BLEND);
    }

    // -------------------- RIVER BANK (soil strips) --------------------
    glColor3f(0.45f, 0.35f, 0.20f);
    glBegin(GL_QUADS);
        glVertex2f(0, topY);
        glVertex2f(WIDTH, topY);
        glVertex2f(WIDTH, topY + 10);
        glVertex2f(0, topY + 10);

        glVertex2f(0, bottomY - 10);
        glVertex2f(WIDTH, bottomY - 10);
        glVertex2f(WIDTH, bottomY);
        glVertex2f(0, bottomY);
    glEnd();
}



void drawRailTrack() {
    float k = 0.55f + 0.45f * dayNightBlend;   // day/night brightness

    float y0 = 335.0f;   // ballast bottom
    float y1 = 360.0f;   // ballast top

    // ---------- BALLAST (stone base) ----------
    glBegin(GL_QUADS);
        glColor3f(0.25f * k, 0.22f * k, 0.20f * k);
        glVertex2f(0, y0);
        glVertex2f(WIDTH, y0);
        glColor3f(0.35f * k, 0.32f * k, 0.28f * k);
        glVertex2f(WIDTH, y1);
        glVertex2f(0, y1);
    glEnd();

    // ---------- SLEEPERS (wooden ties) ----------
    float sleeperY0 = y0 + 5;
    float sleeperY1 = y1 - 5;

    for (float x = 0; x < WIDTH; x += 45.0f) {
        // Wooden sleeper
        glColor3f(0.40f * k, 0.28f * k, 0.18f * k);
        glBegin(GL_QUADS);
            glVertex2f(x,      sleeperY0);
            glVertex2f(x+30.0f, sleeperY0);
            glVertex2f(x+30.0f, sleeperY1);
            glVertex2f(x,      sleeperY1);
        glEnd();
        
        // Sleeper outline
        glColor3f(0.20f * k, 0.12f * k, 0.08f * k);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x,      sleeperY0);
            glVertex2f(x+30.0f, sleeperY0);
            glVertex2f(x+30.0f, sleeperY1);
            glVertex2f(x,      sleeperY1);
        glEnd();
    }

    // ---------- RAILS ----------
    float railY1 = 347.0f;
    float railY2 = 354.0f;

    // Left rail
    glColor3f(0.60f * k, 0.60f * k, 0.65f * k);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
        glVertex2f(0, railY1);
        glVertex2f(WIDTH, railY1);
    glEnd();
    
    // Right rail
    glBegin(GL_LINES);
        glVertex2f(0, railY2);
        glVertex2f(WIDTH, railY2);
    glEnd();
    
    // Rail highlights
    glColor3f(0.85f * k, 0.85f * k, 0.90f * k);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(0, railY1 + 1.5f);
        glVertex2f(WIDTH, railY1 + 1.5f);
        glVertex2f(0, railY2 + 1.5f);
        glVertex2f(WIDTH, railY2 + 1.5f);
    glEnd();
    
    // Rail shadows
    glColor3f(0.12f * k, 0.12f * k, 0.12f * k);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
        glVertex2f(0, railY1 - 2);
        glVertex2f(WIDTH, railY1 - 2);
        glVertex2f(0, railY2 - 2);
        glVertex2f(WIDTH, railY2 - 2);
    glEnd();

    // ---------- RAIL CONNECTORS (small vertical lines between rails) ----------
    glColor3f(0.50f * k, 0.50f * k, 0.55f * k);
    glLineWidth(3.0f);
    for (float x = 22.0f; x < WIDTH; x += 45.0f) {
        glBegin(GL_LINES);
            glVertex2f(x, railY1);
            glVertex2f(x, railY2);
        glEnd();
    }
}

// ============================================================================
// DIVERSE HOUSES
// ============================================================================

void drawModernHouse(float x, float y) {
    // Body
    glColor3f(0.45f, 0.65f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+100, y);
        glVertex2f(x+100, y+80);
        glVertex2f(x, y+80);
    glEnd();

    // Roof
    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-10, y+80);
        glVertex2f(x+50, y+120);
        glVertex2f(x+110, y+80);
    glEnd();

    // Door
    glColor3f(0.85f, 0.70f, 0.50f);
    glBegin(GL_QUADS);
        glVertex2f(x+42, y);
        glVertex2f(x+58, y);
        glVertex2f(x+58, y+45);
        glVertex2f(x+42, y+45);
    glEnd();

    // Window left
    glColor3f(0.75f, 0.90f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x+15, y+45);
        glVertex2f(x+30, y+45);
        glVertex2f(x+30, y+65);
        glVertex2f(x+15, y+65);
    glEnd();

    // Window right
    glBegin(GL_QUADS);
        glVertex2f(x+70, y+45);
        glVertex2f(x+85, y+45);
        glVertex2f(x+85, y+65);
        glVertex2f(x+70, y+65);
    glEnd();

    // Window frames
    glColor3f(0.25f, 0.25f, 0.25f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x+15, y+45); glVertex2f(x+30, y+45);
        glVertex2f(x+30, y+65); glVertex2f(x+15, y+65);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2f(x+70, y+45); glVertex2f(x+85, y+45);
        glVertex2f(x+85, y+65); glVertex2f(x+70, y+65);
    glEnd();

    // Door knob
    glColor3f(0.95f, 0.90f, 0.20f);
    drawCircle(x+55, y+25, 2.0f);

    // Outline
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x+100, y);
        glVertex2f(x+100, y+80); glVertex2f(x, y+80);
    glEnd();
}

void drawTraditionalHouse(float x, float y) {
    // Body
    glColor3f(0.92f, 0.74f, 0.52f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+100, y);
        glVertex2f(x+100, y+80);
        glVertex2f(x, y+80);
    glEnd();

    // Roof
    glColor3f(0.55f, 0.25f, 0.18f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-10, y+80);
        glVertex2f(x+50, y+120);
        glVertex2f(x+110, y+80);
    glEnd();

    // Windows (left/right)
    glColor3f(0.88f, 0.95f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x+15, y+45); glVertex2f(x+35, y+45);
        glVertex2f(x+35, y+65); glVertex2f(x+15, y+65);

        glVertex2f(x+65, y+45); glVertex2f(x+85, y+45);
        glVertex2f(x+85, y+65); glVertex2f(x+65, y+65);
    glEnd();

    // Window frames
    glColor3f(0.25f, 0.25f, 0.25f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x+15, y+45); glVertex2f(x+35, y+45);
        glVertex2f(x+35, y+65); glVertex2f(x+15, y+65);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2f(x+65, y+45); glVertex2f(x+85, y+45);
        glVertex2f(x+85, y+65); glVertex2f(x+65, y+65);
    glEnd();

    // Door (center)
    glColor3f(0.45f, 0.28f, 0.18f);
    glBegin(GL_QUADS);
        glVertex2f(x+42, y);
        glVertex2f(x+58, y);
        glVertex2f(x+58, y+45);
        glVertex2f(x+42, y+45);
    glEnd();

    // Door knob
    glColor3f(0.95f, 0.90f, 0.20f);
    drawCircle(x+55, y+25, 2.0f);

    // Outline
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x+100, y);
        glVertex2f(x+100, y+80); glVertex2f(x, y+80);
    glEnd();
}

void drawFarmHouse(float x, float y) {
    // Body
    glColor3f(0.85f, 0.62f, 0.40f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+100, y);
        glVertex2f(x+100, y+80);
        glVertex2f(x, y+80);
    glEnd();

    // Roof
    glColor3f(0.70f, 0.18f, 0.12f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-10, y+80);
        glVertex2f(x+50, y+120);
        glVertex2f(x+110, y+80);
    glEnd();

    // Door
    glColor3f(0.45f, 0.28f, 0.18f);
    glBegin(GL_QUADS);
        glVertex2f(x+42, y);
        glVertex2f(x+58, y);
        glVertex2f(x+58, y+45);
        glVertex2f(x+42, y+45);
    glEnd();

    // Window left
    glColor3f(0.80f, 0.93f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x+15, y+45);
        glVertex2f(x+30, y+45);
        glVertex2f(x+30, y+65);
        glVertex2f(x+15, y+65);
    glEnd();

    // Window right
    glBegin(GL_QUADS);
        glVertex2f(x+70, y+45);
        glVertex2f(x+85, y+45);
        glVertex2f(x+85, y+65);
        glVertex2f(x+70, y+65);
    glEnd();

    // Window frames
    glColor3f(0.25f, 0.25f, 0.25f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x+15, y+45); glVertex2f(x+30, y+45);
        glVertex2f(x+30, y+65); glVertex2f(x+15, y+65);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2f(x+70, y+45); glVertex2f(x+85, y+45);
        glVertex2f(x+85, y+65); glVertex2f(x+70, y+65);
    glEnd();

    // Door knob
    glColor3f(0.95f, 0.90f, 0.20f);
    drawCircle(x+55, y+25, 2.0f);

    // Outline
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x+100, y);
        glVertex2f(x+100, y+80); glVertex2f(x, y+80);
    glEnd();
}

void drawCottage(float x, float y) {
    // Body
    glColor3f(0.95f, 0.88f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+100, y);
        glVertex2f(x+100, y+80);
        glVertex2f(x, y+80);
    glEnd();

    // Roof
    glColor3f(0.60f, 0.40f, 0.22f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-10, y+80);
        glVertex2f(x+50, y+120);
        glVertex2f(x+110, y+80);
    glEnd();

    // Door
    glColor3f(0.40f, 0.25f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(x+42, y);
        glVertex2f(x+58, y);
        glVertex2f(x+58, y+45);
        glVertex2f(x+42, y+45);
    glEnd();

    // Window (single large window)
    glColor3f(0.80f, 0.93f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x+30, y+45);
        glVertex2f(x+70, y+45);
        glVertex2f(x+70, y+65);
        glVertex2f(x+30, y+65);
    glEnd();

    // Window frame
    glColor3f(0.25f, 0.25f, 0.25f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x+30, y+45); glVertex2f(x+70, y+45);
        glVertex2f(x+70, y+65); glVertex2f(x+30, y+65);
    glEnd();
    
    // Window cross
    glBegin(GL_LINES);
        glVertex2f(x+50, y+45); glVertex2f(x+50, y+65);
        glVertex2f(x+30, y+55); glVertex2f(x+70, y+55);
    glEnd();

    // Door knob
    glColor3f(0.95f, 0.90f, 0.20f);
    drawCircle(x+55, y+25, 2.0f);

    // Outline
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x+100, y);
        glVertex2f(x+100, y+80); glVertex2f(x, y+80);
    glEnd();
}

// ============================================================================
// VEGETATION
// ============================================================================

void drawTree(float x, float y, int type) {
    // brightness factor for day/night (0.55..1.0)
    float k = 0.55f + 0.45f * dayNightBlend;

    // small wind sway (subtle movement)
    float sway = std::sin(riverWave * 0.03f + x * 0.01f) * 4.0f * windIntensity;

    // ground shadow (helps realism a lot)
    drawShadowEllipse(x, y - 6, 40, 11, 0.22f);

    // ---- helper: taper trunk + bark + depth strip ----
    auto trunkTaper = [&](float baseW, float topW, float h) {
        // main trunk
        glColor3f(0.40f * k, 0.25f * k, 0.12f * k);
        glBegin(GL_POLYGON);
            glVertex2f(x - baseW, y);
            glVertex2f(x + baseW, y);
            glVertex2f(x + topW,  y + h);
            glVertex2f(x - topW,  y + h);
        glEnd();

        // right shadow strip (depth)
        glColor3f(0.26f * k, 0.16f * k, 0.08f * k);
        glBegin(GL_QUADS);
            glVertex2f(x + baseW * 0.25f, y + 2);
            glVertex2f(x + baseW,        y + 2);
            glVertex2f(x + topW,         y + h - 2);
            glVertex2f(x + topW * 0.25f, y + h - 2);
        glEnd();

        // bark lines
        glColor3f(0.18f * k, 0.12f * k, 0.06f * k);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < 6; i++) {
            float xx = x - baseW * 0.6f + i * (baseW * 0.25f);
            glVertex2f(xx, y + 6);
            glVertex2f(xx + 2, y + h - 10);
        }
        glEnd();
    };

    // ---- helper: leaf cluster with shading + highlight ----
    auto leafCluster = [&](float cx, float cy, float r, float darkMul, float lightMul) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // dark bottom mass
        glColor4f(0.08f * k * darkMul, 0.40f * k * darkMul, 0.10f * k * darkMul, 1.0f);
        drawCircle(cx, cy - r * 0.25f, r, 26);
        drawCircle(cx - r * 0.75f, cy - r * 0.15f, r * 0.85f, 24);
        drawCircle(cx + r * 0.75f, cy - r * 0.15f, r * 0.85f, 24);

        // lighter top mass
        glColor4f(0.18f * k * lightMul, 0.62f * k * lightMul, 0.18f * k * lightMul, 1.0f);
        drawCircle(cx, cy + r * 0.25f, r * 0.92f, 26);
        drawCircle(cx - r * 0.65f, cy + r * 0.15f, r * 0.75f, 24);
        drawCircle(cx + r * 0.65f, cy + r * 0.15f, r * 0.75f, 24);

        // highlight (volume)
        glColor4f(1.0f, 1.0f, 1.0f, 0.10f);
        drawEllipse(cx + r * 0.25f, cy + r * 0.45f, r * 0.55f, r * 0.25f, 24);

        glDisable(GL_BLEND);
    };

    int t = type % 4;

    // ==========================================================
    // 0) PINE (layered triangles + slight gradient)
    // ==========================================================
    if (t == 0) {
        trunkTaper(7, 4, 55);

        float baseY = y + 35;
        for (int i = 0; i < 4; i++) {
            float layerH = 30.0f - i * 3.0f;
            float w      = 48.0f - i * 10.0f;

            float dark = (0.18f - i * 0.01f) * k;
            float mid  = (0.45f - i * 0.02f) * k;

            glBegin(GL_TRIANGLES);
                glColor3f(0.06f * k, mid, 0.08f * k);
                glVertex2f(x, baseY + i * 18 + layerH);

                glColor3f(0.05f * k, dark, 0.07f * k);
                glVertex2f(x - w, baseY + i * 18);

                glColor3f(0.08f * k, (mid + 0.08f) * k, 0.10f * k);
                glVertex2f(x + w, baseY + i * 18);
            glEnd();
        }

        // tiny top highlight
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 0.08f);
        drawEllipse(x + 10, y + 95, 12, 5, 18);
        glDisable(GL_BLEND);
    }

    // ==========================================================
    // 1) DECIDUOUS (round realistic tree)
    // ==========================================================
    else if (t == 1) {
        trunkTaper(9, 5, 65);

        leafCluster(x + sway * 0.2f, y + 88 + sway, 36.0f, 1.00f, 1.00f);
        leafCluster(x - 26 + sway * 0.15f, y + 72 + sway * 0.6f, 28.0f, 1.05f, 0.98f);
        leafCluster(x + 26 + sway * 0.15f, y + 72 + sway * 0.6f, 28.0f, 1.05f, 0.98f);

        // branches
        glColor3f(0.28f * k, 0.17f * k, 0.08f * k);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex2f(x, y + 45); glVertex2f(x - 18, y + 62);
            glVertex2f(x, y + 50); glVertex2f(x + 18, y + 65);
        glEnd();
    }

    // ==========================================================
    // 2) PALM (FIXED: filled leaves, no spiky line-star)
    // ==========================================================
    else if (t == 2) {
        // palm shadow a bit wider
        drawShadowEllipse(x, y - 6, 50, 12, 0.22f);

        // trunk segments (tapered)
        float h = 110.0f;
        float baseW = 10.0f;

        for (int i = 0; i < 10; i++) {
            float yy1 = y + i * (h / 10.0f);
            float yy2 = yy1 + (h / 10.0f);

            float w1 = baseW - i * 0.45f;
            float w2 = baseW - (i + 1) * 0.45f;

            float col = (i % 2 == 0) ? 0.42f : 0.36f;
            glColor3f(col * k, 0.26f * k, 0.13f * k);

            glBegin(GL_QUADS);
                glVertex2f(x - w1, yy1);
                glVertex2f(x + w1, yy1);
                glVertex2f(x + w2, yy2);
                glVertex2f(x - w2, yy2);
            glEnd();
        }

        // depth strip
        glColor3f(0.25f * k, 0.16f * k, 0.08f * k);
        glBegin(GL_QUADS);
            glVertex2f(x + 2.0f, y + 5);
            glVertex2f(x + 8.0f, y + 5);
            glVertex2f(x + 3.0f, y + h);
            glVertex2f(x - 1.0f, y + h);
        glEnd();

        float topX = x;
        float topY = y + h;

        // coconuts
        glColor3f(0.28f * k, 0.20f * k, 0.10f * k);
        drawCircle(topX - 7, topY - 10, 5, 16);
        drawCircle(topX + 2, topY - 12, 5, 16);
        drawCircle(topX + 9, topY - 9,  4, 16);

        float palmSway = std::sin(riverWave * 0.03f + x * 0.01f) * 6.0f * windIntensity;

        // one filled palm leaf (2 triangles) + highlight + midrib
        auto palmLeaf = [&](float angDeg, float len, float w) {
            float a = (angDeg + palmSway) * (3.1415926f / 180.0f);
            float cx = std::cos(a), syy = std::sin(a);

            float ex = topX + cx * len;
            float ey = topY + syy * (len * 0.55f);

            float px = -syy, py = cx; // perpendicular

            float bW = w;
            float tW = w * 0.15f;

            // dark leaf body
            glColor3f(0.08f * k, 0.45f * k, 0.12f * k);
            glBegin(GL_TRIANGLES);
                glVertex2f(topX, topY);
                glVertex2f(topX + px*bW, topY + py*bW);
                glVertex2f(ex + px*tW,   ey + py*tW);

                glVertex2f(topX, topY);
                glVertex2f(ex - px*tW,   ey - py*tW);
                glVertex2f(topX - px*bW, topY - py*bW);
            glEnd();

            // light highlight
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.30f * k, 0.85f * k, 0.30f * k, 0.35f);
            glBegin(GL_TRIANGLES);
                glVertex2f(topX, topY);
                glVertex2f(topX + px*(bW*0.45f), topY + py*(bW*0.45f));
                glVertex2f(ex + px*(tW*0.6f),    ey + py*(tW*0.6f));
            glEnd();
            glDisable(GL_BLEND);

            // midrib
            glColor3f(0.05f * k, 0.30f * k, 0.08f * k);
            glLineWidth(2.0f);
            glBegin(GL_LINES);
                glVertex2f(topX, topY);
                glVertex2f(ex, ey);
            glEnd();
        };

        // leaves
        palmLeaf( 20, 78, 12);
        palmLeaf( 55, 70, 11);
        palmLeaf( 95, 62, 10);
        palmLeaf(140, 70, 11);
        palmLeaf(170, 78, 12);
        palmLeaf(-20, 78, 12);
        palmLeaf(-55, 70, 11);
        palmLeaf(-95, 62, 10);
    }

    // ==========================================================
    // 3) FRUIT TREE (canopy + fruits + highlight)
    // ==========================================================
    else {
        trunkTaper(10, 6, 70);

        leafCluster(x + sway * 0.2f, y + 95 + sway * 0.9f, 34.0f, 1.00f, 1.05f);
        leafCluster(x - 22 + sway * 0.15f, y + 78 + sway * 0.6f, 26.0f, 1.02f, 1.00f);
        leafCluster(x + 22 + sway * 0.15f, y + 78 + sway * 0.6f, 26.0f, 1.02f, 1.00f);

        // fruits
        glColor3f(1.0f * k, 0.50f * k, 0.05f * k);
        drawCircle(x - 10, y + 88, 4, 16);
        drawCircle(x +  6, y + 84, 4, 16);
        drawCircle(x + 14, y + 96, 4, 16);

        // fruit highlights
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 0.18f);
        drawCircle(x - 11, y + 90, 1.2f, 10);
        drawCircle(x +  5, y + 86, 1.2f, 10);
        glDisable(GL_BLEND);
    }
}


void drawBush(float x, float y, float size) {
    glColor3f(0.1f, 0.4f, 0.1f);
    drawCircle(x, y, size);
    drawCircle(x + size * 0.8f, y, size * 0.7f);
    drawCircle(x - size * 0.8f, y, size * 0.7f);
    drawCircle(x, y + size * 0.7f, size * 0.6f);
}

// ============================================================================
// VEHICLES
// ============================================================================

// ======================= REPLACE THIS FUNCTION =======================
// Cleaner engine "first bogie": grill looks nicer + less weird when covered
void drawTrain(float x, float y) {
    int bogieCount = trainBogieCount;

    // ============================================================
    // 1) SOFT GROUND SHADOWS (NO BIG RECTANGLE SHADOW)
    //    (This fixes the weird big "shape" shadow)
    // ============================================================
    // Engine ground shadow
    drawShadowEllipse(x + 75,  y - 14, 78, 12, 0.22f);

    // Extra small shadow near engine back bogie (connector side)
    // (This is the one you asked: "engine bogie back side shadow")
    drawShadowEllipse(x + 138, y - 13, 26, 9,  0.18f);

    // Coach ground shadows
    float shadowX = x + 170;
    for (int i = 0; i < bogieCount; i++) {
        drawShadowEllipse(shadowX + 45, y - 14, 55, 10, 0.18f);
        shadowX += 100;
    }

    // -------------------- ENGINE (front) --------------------

    // Main engine body
    glColor3f(0.70f, 0.10f, 0.12f);
    glBegin(GL_QUADS);
        glVertex2f(x,     y);
        glVertex2f(x+120, y);
        glVertex2f(x+120, y+48);
        glVertex2f(x,     y+48);
    glEnd();

    // Engine back-side shading strip (near coach connection side)
    // (gives depth + fixes flat look)
    glColor3f(0.52f, 0.07f, 0.09f);
    glBegin(GL_QUADS);
        glVertex2f(x+104, y+2);
        glVertex2f(x+120, y+2);
        glVertex2f(x+120, y+46);
        glVertex2f(x+104, y+46);
    glEnd();

    // Engine nose (angled)
    glColor3f(0.60f, 0.08f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(x+120, y);
        glVertex2f(x+150, y+8);
        glVertex2f(x+150, y+40);
        glVertex2f(x+120, y+48);
    glEnd();

    // Roof
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
        glVertex2f(x+10,  y+48);
        glVertex2f(x+115, y+48);
        glVertex2f(x+108, y+62);
        glVertex2f(x+15,  y+62);
    glEnd();

    // Window (engine)
    glColor3f(0.80f, 0.93f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x+20, y+22);
        glVertex2f(x+45, y+22);
        glVertex2f(x+45, y+40);
        glVertex2f(x+20, y+40);
    glEnd();

    // Door (engine)
    glColor3f(0.40f, 0.05f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(x+55, y+10);
        glVertex2f(x+78, y+10);
        glVertex2f(x+78, y+40);
        glVertex2f(x+55, y+40);
    glEnd();

    // Front grill lines
    glColor3f(0.20f, 0.20f, 0.20f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        for (int i = 0; i < 6; i++) {
            float gx = x + 92 + i * 4.0f;
            glVertex2f(gx, y+8);
            glVertex2f(gx, y+40);
        }
    glEnd();

    // Headlight (glow at night)
    if (!isDay) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1.0f, 1.0f, 0.85f, 0.85f);
        drawCircle(x+148, y+24, 5.0f, 18);

        glColor4f(1.0f, 1.0f, 0.85f, 0.18f);
        drawCircle(x+148, y+24, 14.0f, 28);

        glDisable(GL_BLEND);
    } else {
        glColor3f(1.0f, 1.0f, 0.85f);
        drawCircle(x+148, y+24, 4.0f, 18);
    }

    // Stripe on engine
    glColor3f(0.95f, 0.85f, 0.20f);
    glBegin(GL_QUADS);
        glVertex2f(x+0,   y+18);
        glVertex2f(x+150, y+18);
        glVertex2f(x+150, y+24);
        glVertex2f(x+0,   y+24);
    glEnd();

    // ============================================================
    // ENGINE BOGIE BASE (THIS REMOVES "WEIRD SHAPE" LOOK)
    // + extra shadow on back side of bogie frame
    // ============================================================
    // Main bogie frame
    glColor3f(0.20f, 0.20f, 0.22f);
    glBegin(GL_QUADS);
        glVertex2f(x + 18, y - 4);
        glVertex2f(x + 108, y - 4);
        glVertex2f(x + 108, y + 6);
        glVertex2f(x + 18, y + 6);
    glEnd();

    // Back-side bogie shadow strip (connector side)
    glColor3f(0.12f, 0.12f, 0.14f);
    glBegin(GL_QUADS);
        glVertex2f(x + 92,  y - 4);
        glVertex2f(x + 108, y - 4);
        glVertex2f(x + 108, y + 6);
        glVertex2f(x + 92,  y + 6);
    glEnd();

    // Coupler (engine to coach)
    glColor3f(0.15f, 0.15f, 0.15f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
        glVertex2f(x+150, y+10);
        glVertex2f(x+165, y+10);
    glEnd();

    // -------------------- COACHES --------------------
    float bx = x + 170;

    for (int i = 0; i < bogieCount; i++) {
        float r = 0.10f + 0.02f * (i % 3);
        float g = 0.35f + 0.03f * (i % 2);
        float b = 0.65f + 0.02f * (i % 3);

        glColor3f(r, g, b);
        glBegin(GL_QUADS);
            glVertex2f(bx,      y);
            glVertex2f(bx+90,   y);
            glVertex2f(bx+90,   y+40);
            glVertex2f(bx,      y+40);
        glEnd();

        // Coach roof
        glColor3f(0.30f, 0.30f, 0.30f);
        glBegin(GL_QUADS);
            glVertex2f(bx+6,   y+40);
            glVertex2f(bx+84,  y+40);
            glVertex2f(bx+78,  y+52);
            glVertex2f(bx+12,  y+52);
        glEnd();

        // Windows (3)
        glColor3f(0.85f, 0.95f, 1.0f);
        for (int w = 0; w < 3; w++) {
            float wx = bx + 12 + w * 25;
            glBegin(GL_QUADS);
                glVertex2f(wx,     y+18);
                glVertex2f(wx+18,  y+18);
                glVertex2f(wx+18,  y+34);
                glVertex2f(wx,     y+34);
            glEnd();
        }

        // Window frames
        glColor3f(0.15f, 0.15f, 0.15f);
        glLineWidth(2.0f);
        for (int w = 0; w < 3; w++) {
            float wx = bx + 12 + w * 25;
            glBegin(GL_LINE_LOOP);
                glVertex2f(wx,     y+18);
                glVertex2f(wx+18,  y+18);
                glVertex2f(wx+18,  y+34);
                glVertex2f(wx,     y+34);
            glEnd();
        }

        // Door
        glColor3f(0.20f, 0.20f, 0.22f);
        glBegin(GL_QUADS);
            glVertex2f(bx+72, y+8);
            glVertex2f(bx+86, y+8);
            glVertex2f(bx+86, y+36);
            glVertex2f(bx+72, y+36);
        glEnd();

        // Connector between coaches
        glColor3f(0.15f, 0.15f, 0.15f);
        glLineWidth(4.0f);
        glBegin(GL_LINES);
            glVertex2f(bx+90,  y+10);
            glVertex2f(bx+100, y+10);
        glEnd();

        bx += 100;
    }

    // -------------------- WHEELS --------------------
    auto drawWheelPair = [&](float wx) {
        glColor3f(0.08f, 0.08f, 0.08f);
        drawCircle(wx,      y - 8, 8, 20);
        drawCircle(wx + 34, y - 8, 8, 20);

        glColor3f(0.75f, 0.75f, 0.75f);
        drawCircle(wx,      y - 8, 3, 16);
        drawCircle(wx + 34, y - 8, 3, 16);
    };

    // Engine wheels (2 pairs)
    drawWheelPair(x + 30);
    drawWheelPair(x + 85);

    // Coach wheels (1 pair per coach)
    float wx = x + 190;
    for (int i = 0; i < bogieCount; i++) {
        drawWheelPair(wx + 18);
        wx += 100;
    }

    // -------------------- OUTLINE --------------------
    glColor3f(0.12f, 0.12f, 0.12f);
    glLineWidth(2.0f);

    // Engine outline
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x+120, y);
        glVertex2f(x+120, y+48); glVertex2f(x, y+48);
    glEnd();

    // Coaches outline
    float ox = x + 170;
    for (int i = 0; i < bogieCount; i++) {
        glBegin(GL_LINE_LOOP);
            glVertex2f(ox, y); glVertex2f(ox+90, y);
            glVertex2f(ox+90, y+40); glVertex2f(ox, y+40);
        glEnd();
        ox += 100;
    }
}

//=======Draw Bicycle========//
void drawCar(float x, float y) {
    // shadow
    drawShadowEllipse(x + 30, y + 2, 22, 4, 0.28f);

    // ===================== WHEELS =====================
    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(x + 12, y, 15, 32);   // rear wheel
    drawCircle(x + 59, y, 15, 32);   // front wheel

    glColor3f(0.55f, 0.55f, 0.55f);
    drawCircle(x + 12, y, 12, 32);
    drawCircle(x + 59, y, 12, 32);

    glColor3f(0.75f, 0.75f, 0.75f);
    drawCircle(x + 12, y, 2, 16);
    drawCircle(x + 59, y, 2, 16);

    // Spokes (rear)
    glColor3f(0.45f, 0.45f, 0.45f);
    glLineWidth(0.8f);
    glBegin(GL_LINES);
        glVertex2f(x + 12, y - 12); glVertex2f(x + 12, y + 12);
        glVertex2f(x,       y);     glVertex2f(x + 24, y);
        glVertex2f(x + 4,  y - 9);  glVertex2f(x + 20, y + 9);
        glVertex2f(x + 4,  y + 9);  glVertex2f(x + 20, y - 9);
    glEnd();
    // Spokes (front)
    glBegin(GL_LINES);
        glVertex2f(x + 59, y - 12); glVertex2f(x + 59, y + 12);
        glVertex2f(x + 47, y);      glVertex2f(x + 71, y);
        glVertex2f(x + 51, y - 9);  glVertex2f(x + 67, y + 9);
        glVertex2f(x + 51, y + 9);  glVertex2f(x + 67, y - 9);
    glEnd();

    // ===================== FRAME =====================
    // BB at (x+35, y+12)
    glColor3f(0.12f, 0.43f, 0.71f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(x + 35, y + 12); glVertex2f(x + 12, y);      // chainstay
        glVertex2f(x + 35, y + 12); glVertex2f(x + 29, y);      // seat tube
        glVertex2f(x + 35, y + 12); glVertex2f(x + 59, y);      // down tube
        glVertex2f(x + 35, y + 12); glVertex2f(x + 54, y + 21); // top tube
        glVertex2f(x + 54, y + 21); glVertex2f(x + 59, y);      // fork
    glEnd();

    // ===================== HANDLEBAR =====================
    glColor3f(0.18f, 0.18f, 0.18f);
    glLineWidth(1.8f);
    glBegin(GL_LINES);
        glVertex2f(x + 54, y + 21); glVertex2f(x + 56, y + 32); // stem
        glVertex2f(x + 51, y + 32); glVertex2f(x + 62, y + 32); // bar
    glEnd();

    // ===================== SEAT POST + SADDLE =====================
    glLineWidth(1.8f);
    glBegin(GL_LINES);
        glVertex2f(x + 29, y); glVertex2f(x + 27, y + 18);
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(x + 22, y + 18); glVertex2f(x + 33, y + 18);
    glEnd();

    // ===================== PEDAL CRANK =====================
    glColor3f(0.55f, 0.55f, 0.55f);
    drawCircle(x + 35, y + 12, 6, 18);  // chainring

    glColor3f(0.32f, 0.32f, 0.32f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
        glVertex2f(x + 35, y + 12); glVertex2f(x + 30, y + 7);  // forward crank
        glVertex2f(x + 35, y + 12); glVertex2f(x + 41, y + 17); // rear crank
    glEnd();

    glColor3f(0.22f, 0.22f, 0.22f);
    glBegin(GL_QUADS);
        glVertex2f(x + 27, y + 8);  glVertex2f(x + 32, y + 8);
        glVertex2f(x + 32, y + 6);  glVertex2f(x + 27, y + 6);  // forward pedal
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 39, y + 16); glVertex2f(x + 44, y + 16);
        glVertex2f(x + 44, y + 18); glVertex2f(x + 39, y + 18); // rear pedal
    glEnd();

    // Chain
    glColor3f(0.35f, 0.35f, 0.35f);
    glLineWidth(0.8f);
    glBegin(GL_LINES);
        glVertex2f(x + 30, y + 10); glVertex2f(x + 12, y + 2);
        glVertex2f(x + 30, y + 14); glVertex2f(x + 12, y - 2);
    glEnd();

    // ===================== RIDER =====================
    glColor3f(0.80f, 0.27f, 0.00f);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
        glVertex2f(x + 27, y + 18); glVertex2f(x + 54, y + 27); // torso
    glEnd();
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glVertex2f(x + 54, y + 27); glVertex2f(x + 56, y + 32); // arms
    glEnd();

    // Right leg
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glVertex2f(x + 27, y + 18); glVertex2f(x + 32, y + 11);
        glVertex2f(x + 32, y + 11); glVertex2f(x + 30, y + 7);
    glEnd();

    // Left leg
    glColor3f(0.60f, 0.20f, 0.00f);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glVertex2f(x + 27, y + 18); glVertex2f(x + 38, y + 15);
        glVertex2f(x + 38, y + 15); glVertex2f(x + 41, y + 17);
    glEnd();

    // Head
    glColor3f(0.96f, 0.75f, 0.54f);
    drawCircle(x + 59, y + 38, 5, 20);

    // Helmet
    glColor3f(0.80f, 0.27f, 0.00f);
    glBegin(GL_POLYGON);
        glVertex2f(x + 54, y + 37);
        glVertex2f(x + 55, y + 44);
        glVertex2f(x + 59, y + 46);
        glVertex2f(x + 63, y + 44);
        glVertex2f(x + 64, y + 37);
    glEnd();
    glColor3f(0.18f, 0.18f, 0.18f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(x + 54, y + 37); glVertex2f(x + 64, y + 37);
    glEnd();

    // ===================== LIGHTS =====================
    if (!isDay) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.85f, 0.85f);
        drawCircle(x + 63, y + 18, 1.6f, 16);
        glColor4f(1.0f, 1.0f, 0.85f, 0.18f);
        drawCircle(x + 63, y + 18, 4.4f, 24);
        glDisable(GL_BLEND);
    } else {
        glColor3f(1.0f, 1.0f, 0.85f);
        drawCircle(x + 63, y + 18, 1.3f, 16);
    }

    // Tail light
    glColor3f(0.95f, 0.20f, 0.20f);
    drawCircle(x + 9, y + 6, 1.1f, 14);

    // ===================== OUTLINE =====================
    glColor3f(0.08f, 0.30f, 0.55f);
    glLineWidth(0.8f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x + 12, y);
        glVertex2f(x + 35, y + 12);
        glVertex2f(x + 54, y + 21);
        glVertex2f(x + 59, y);
        glVertex2f(x + 54, y + 21);
        glVertex2f(x + 35, y + 12);
        glVertex2f(x + 29, y);
    glEnd();
}

// ======================= MORE REALISTIC BOAT (WITH REFLECTION) ==============

void drawBoat(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // ------------------- WATER SHADOW -------------------
    drawShadowEllipse(55.0f, -10.0f, 55.0f, 8.5f, 0.28f);

    // ------------------- HULL (curved + depth) -------------------
    // top hull
    glColor3f(0.42f, 0.22f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(  6,  4);
        glVertex2f( 18,  0);
        glVertex2f( 90,  0);
        glVertex2f(106,  6);
        glVertex2f( 96, 22);
        glVertex2f( 20, 22);
    glEnd();

    // underside darker strip (depth)
    glColor3f(0.28f, 0.14f, 0.06f);
    glBegin(GL_POLYGON);
        glVertex2f( 16,  0);
        glVertex2f( 92,  0);
        glVertex2f(102,  6);
        glVertex2f( 96, 14);
        glVertex2f( 22, 14);
        glVertex2f( 18,  6);
    glEnd();

    // upper highlight strip (shine)
    glColor3f(0.58f, 0.33f, 0.16f);
    glBegin(GL_QUADS);
        glVertex2f(18, 18);
        glVertex2f(94, 18);
        glVertex2f(90, 22);
        glVertex2f(22, 22);
    glEnd();

    // ------------------- CABIN (better) -------------------
    glColor3f(0.68f, 0.40f, 0.20f);
    glBegin(GL_QUADS);
        glVertex2f(34, 24);
        glVertex2f(76, 24);
        glVertex2f(76, 50);
        glVertex2f(34, 50);
    glEnd();

    // cabin shading strip (depth)
    glColor3f(0.50f, 0.28f, 0.14f);
    glBegin(GL_QUADS);
        glVertex2f(70, 24);
        glVertex2f(76, 24);
        glVertex2f(76, 50);
        glVertex2f(70, 50);
    glEnd();

    // roof
    glColor3f(0.93f, 0.93f, 0.93f);
    glBegin(GL_QUADS);
        glVertex2f(32, 50);
        glVertex2f(78, 50);
        glVertex2f(74, 58);
        glVertex2f(36, 58);
    glEnd();

    // windows (glass)
    glColor3f(0.80f, 0.93f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(38, 34); glVertex2f(52, 34);
        glVertex2f(52, 46); glVertex2f(38, 46);

        glVertex2f(54, 34); glVertex2f(72, 34);
        glVertex2f(72, 46); glVertex2f(54, 46);
    glEnd();

    // window frames
    glColor3f(0.15f, 0.15f, 0.15f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(38, 34); glVertex2f(52, 34);
        glVertex2f(52, 46); glVertex2f(38, 46);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2f(54, 34); glVertex2f(72, 34);
        glVertex2f(72, 46); glVertex2f(54, 46);
    glEnd();

    // window split line
    glBegin(GL_LINES);
        glVertex2f(52, 34);
        glVertex2f(52, 46);
    glEnd();

    // ------------------- SMALL DETAILS -------------------
    // railing line
    glColor3f(0.85f, 0.85f, 0.85f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(20, 24);
        glVertex2f(96, 24);
    glEnd();

    // portholes on hull (3 circles)
    glColor3f(0.18f, 0.18f, 0.18f);
    drawCircle(34, 12, 3.0f, 18);
    drawCircle(52, 12, 3.0f, 18);
    drawCircle(70, 12, 3.0f, 18);

    glColor3f(0.78f, 0.92f, 1.0f);
    drawCircle(34, 12, 1.2f, 14);
    drawCircle(52, 12, 1.2f, 14);
    drawCircle(70, 12, 1.2f, 14);

    // mast + flag (a bit more realistic)
    glColor3f(0.20f, 0.20f, 0.22f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(76, 58);
        glVertex2f(76, 74);
    glEnd();

    glColor3f(0.95f, 0.20f, 0.20f);
    glBegin(GL_TRIANGLES);
        glVertex2f(76, 72);
        glVertex2f(92, 66);
        glVertex2f(76, 60);
    glEnd();

    // life ring (cute detail)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.55f, 0.10f, 0.90f);
    drawCircle(26, 18, 5.0f, 20);
    glColor4f(0.85f, 0.95f, 1.0f, 0.85f);
    drawCircle(26, 18, 2.5f, 20);
    glDisable(GL_BLEND);

    // ------------------- WATER WAKE (foam trail) -------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.90f, 0.95f, 1.0f, 0.45f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 6; i++) {
            float t = i / 6.0f;
            float wx = 6.0f - t * 35.0f;
            float wy = 10.0f + std::sin(riverWave * 0.12f + t * 6.0f) * 2.0f;
            glVertex2f(wx, wy);
        }
    glEnd();

    glColor4f(0.90f, 0.95f, 1.0f, 0.25f);
    drawEllipse(10.0f, 6.0f, 18.0f, 3.5f, 26);

    glDisable(GL_BLEND);

    // ------------------- OUTLINE (crisp) -------------------
    glColor3f(0.12f, 0.12f, 0.12f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(  6,  4);
        glVertex2f( 18,  0);
        glVertex2f( 90,  0);
        glVertex2f(106,  6);
        glVertex2f( 96, 22);
        glVertex2f( 20, 22);
    glEnd();

    glPopMatrix();
}

// SIMPLE blue reflection shape under boat in river
void drawBoatWaterReflection(float boatX, float boatY) {
    float waterMid = 150.0f;
    float refY     = waterMid - (boatY - waterMid);
    float centerX  = boatX + 55.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // main reflection blob
    glColor4f(0.0f, 0.10f, 0.25f, 0.28f);
    drawEllipse(centerX, refY, 62.0f, 11.0f, 44);

    // ripple layers (animated with riverWave)
    float wob = std::sin(riverWave * 0.10f) * 3.0f;

    glColor4f(0.0f, 0.12f, 0.28f, 0.18f);
    drawEllipse(centerX + 6.0f, refY - 4.0f + wob, 48.0f, 8.0f, 40);

    glColor4f(0.0f, 0.12f, 0.28f, 0.12f);
    drawEllipse(centerX - 8.0f, refY - 10.0f - wob, 36.0f, 6.0f, 36);

    glDisable(GL_BLEND);
}


// Boat wake + foam + ripple rings (synced with riverWave & boatPosition)
void drawBoatWake(float boatX, float boatY) {
    // river bounds (same as your drawRiver)
    float topY    = 180.0f;
    float bottomY = 120.0f;
    float midY    = (topY + bottomY) * 0.5f;

    // boat center
    float cx = boatX + 55.0f;     // boat center X (your boat width ~110)
    float cy = boatY + 6.0f;      // slightly above hull bottom

    // phase: uses both riverWave and boatPosition
    float t = riverWave * 0.10f + boatX * 0.01f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ---------------- 1) Main white foam trail (behind boat) ----------------
    // a few stretched ellipses behind the boat
    for (int i = 0; i < 6; i++) {
        float k  = (float)i;
        float fx = cx - 18.0f - k * 22.0f;              // behind boat
        float fy = midY + std::sin(t + k * 0.7f) * 2.0f;

        // fade out as it goes back
        float a = 0.22f - k * 0.03f;
        if (a < 0.03f) a = 0.03f;

        glColor4f(0.95f, 0.98f, 1.0f, a);
        drawEllipse(fx, fy, 18.0f + k * 2.0f, 3.2f, 28);
    }

    // ---------------- 2) Side foam near hull edges ----------------
    glColor4f(0.90f, 0.95f, 1.0f, 0.14f);
    drawEllipse(cx - 8.0f, cy - 3.0f, 10.0f, 2.5f, 24);
    drawEllipse(cx + 10.0f, cy - 3.0f, 10.0f, 2.5f, 24);

    // ---------------- 3) Ripple rings (expanding circles) ----------------
    // 3 rings that expand with time
    for (int i = 0; i < 3; i++) {
        float phase = std::fmod(riverWave * (1.4f + i * 0.35f), 22.0f);
        float rr    = 8.0f + phase + i * 6.0f;

        float rx = cx - 10.0f - i * 18.0f;  // slightly behind boat
        float ry = midY + std::sin(t + i) * 2.0f;

        float a = 0.10f - i * 0.02f;
        if (a < 0.03f) a = 0.03f;

        glColor4f(0.85f, 0.95f, 1.0f, a);
        drawEllipse(rx, ry, rr, rr * 0.32f, 32);
    }

    glDisable(GL_BLEND);
}


void drawAirplane() {
    float x = std::fmod(planePosition, WIDTH + 400.0f) - 200.0f;
    float y = HEIGHT - 100.0f + 20.0f * std::sin(planePosition * 0.02f);

    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 100, y);
    glVertex2f(x + 100, y + 25);
    glVertex2f(x, y + 25);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(x + 100, y);
    glVertex2f(x + 100, y + 25);
    glVertex2f(x + 120, y + 12.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 25);
    glVertex2f(x - 15, y + 40);
    glVertex2f(x + 15, y + 25);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x + 30, y + 25);
    glVertex2f(x + 70, y + 25);
    glVertex2f(x + 65, y + 45);
    glVertex2f(x + 35, y + 45);
    glVertex2f(x + 10, y + 15);
    glVertex2f(x + 30, y + 15);
    glVertex2f(x + 30, y + 20);
    glVertex2f(x + 10, y + 20);
    glEnd();

    glColor3f(0.6f, 0.9f, 1.0f);
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++) {
        float wx = x + 15 + i * 14;
        glVertex2f(wx, y + 8);
        glVertex2f(wx + 10, y + 8);
        glVertex2f(wx + 10, y + 18);
        glVertex2f(wx, y + 18);
    }
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
    for (int i = 0; i < 3; i++) {
        float trailX    = x - 20 - i * 25;
        float trailSize = 15.0f - i * 4.0f;
        drawCircle(trailX, y + 12.5f, trailSize);
    }
    glDisable(GL_BLEND);
}

void drawFish() {
    auto drawOneFish = [&](float baseX, float baseY, float scale, bool faceRight,
                           float phase, float r, float g, float b) {
        float swim = std::sin(fishPosition * 0.08f + phase) * 3.0f;
        float sway = std::sin(fishPosition * 0.25f + phase) * 2.2f;

        glPushMatrix();
        glTranslatef(baseX, baseY + swim, 0.0f);
        glScalef((faceRight ? 1.0f : -1.0f) * scale, scale, 1.0f);
        glTranslatef(0.0f, sway * 0.12f, 0.0f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(r, g, b, 0.85f);
        drawEllipse(0.0f, 0.0f, 18.0f, 8.0f, 32);

        glColor4f(1.0f, 1.0f, 1.0f, 0.18f);
        drawEllipse(4.0f, 2.0f, 10.0f, 3.0f, 24);

        float tailWag = std::sin(fishPosition * 0.35f + phase) * 3.5f;
        glColor4f(r * 0.95f, g * 0.85f, b * 0.85f, 0.90f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-18.0f, 0.0f);
            glVertex2f(-30.0f, 7.0f + tailWag);
            glVertex2f(-30.0f, -7.0f - tailWag);
        glEnd();

        glColor4f(1.0f, 1.0f, 1.0f, 0.95f);
        drawCircle(12.0f, 3.0f, 2.0f, 16);
        glColor4f(0.0f, 0.0f, 0.0f, 0.95f);
        drawCircle(12.5f, 3.0f, 1.0f, 12);

        float bubbleUp = std::fmod(riverWave * 0.8f + phase * 30.0f, 30.0f);
        glColor4f(0.85f, 0.95f, 1.0f, 0.35f);
        drawCircle(22.0f, 3.0f + bubbleUp, 2.8f, 16);
        drawCircle(30.0f, 8.0f + bubbleUp * 0.7f, 2.0f, 14);
        drawCircle(38.0f, 12.0f + bubbleUp * 0.5f, 1.5f, 12);

        glDisable(GL_BLEND);
        glPopMatrix();
    };

    float x0 = std::fmod(fishPosition, WIDTH + 250.0f) - 125.0f;
    drawOneFish(x0 + 120, 150.0f, 1.00f, true,  0.2f, 1.0f, 0.55f, 0.10f);
    drawOneFish(x0 + 40,  142.0f, 0.75f, true,  1.1f, 0.30f, 0.80f, 0.95f);
    drawOneFish(x0 - 30,  160.0f, 0.65f, true, 2.0f, 0.90f, 0.35f, 0.25f);
    drawOneFish(x0 - 110, 136.0f, 0.55f, true,  2.8f, 0.35f, 0.90f, 0.40f);
}

// ============================================================================
// ANIMATED OBJECTS
// ============================================================================

void drawMovingTrain() {
    float trainX = trainPosition;
    if (trainX > -400 && trainX < WIDTH + 400) {
        drawTrain(trainX, 365);
    }
}

void drawMovingCar() {
    float carX = std::fmod(carPosition, WIDTH + 300.0f) - 150.0f;
    drawCar(carX, 240);
}

void drawMovingBoat() {
    float boatX = std::fmod(boatPosition, WIDTH + 250.0f) - 150.0f;
    float boatY = 125.0f + 4.0f * std::sin(boatPosition * 0.05f);

    // ✅ wake first (so it stays under boat)
    drawBoatWake(boatX, boatY);

    // reflection in water
    drawBoatWaterReflection(boatX, boatY);

    // boat on top
    drawBoat(boatX, 140.0f);
}


void drawBirds() {
    if (!showBirds) return;

    const int numBirds    = 6;
    const int birdsPerRow = 3;      // 3 birds per row
    const float spacingX  = 45.0f;  // left-right spacing
    const float rowGapY   = 28.0f;  // top-bottom row gap
    const float rowBackX  = 25.0f;  // 2nd row slightly behind (more realistic)

    float flockX = std::fmod(birdOffset * 1.5f, WIDTH + 200.0f) - 100.0f;

    for (int i = 0; i < numBirds; i++) {

        // ---- 2-row formation (3 birds each row) ----
        int row = i / birdsPerRow;   // 0 or 1
        int col = i % birdsPerRow;   // 0,1,2

        float bx = flockX + col * spacingX - row * rowBackX;
        float by = 540.0f - row * rowGapY
                 + std::sin(birdOffset * 0.08f + i * 0.5f) * 12.0f;

        float scale = 0.8f + 0.1f * (i % 3);
        float flap  = std::sin(birdOffset * 0.30f + i * 0.8f) * 25.0f;

        glPushMatrix();
        glTranslatef(bx, by, 0.0f);
        glScalef(scale, scale, 1.0f);

        float br = isDay ? 0.22f : 0.12f;
        float bg = isDay ? 0.22f : 0.12f;
        float bb = isDay ? 0.28f : 0.20f;
        glColor3f(br, bg, bb);
        drawEllipse(0.0f, 0.0f, 10.0f, 6.0f, 24);

        glColor3f(0.50f, 0.50f, 0.55f);
        drawEllipse(2.0f, -1.0f, 6.0f, 3.0f, 20);

        glColor3f(0.24f, 0.24f, 0.28f);
        drawCircle(9.0f, 3.0f, 4.0f, 18);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(10.0f, 3.0f, 1.1f, 12);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(10.4f, 3.0f, 0.6f, 10);

        glColor3f(0.95f, 0.65f, 0.10f);
        glBegin(GL_TRIANGLES);
            glVertex2f(12.5f, 2.0f);
            glVertex2f(16.0f, 3.5f);
            glVertex2f(12.5f, 5.0f);
        glEnd();

        glColor3f(0.16f, 0.16f, 0.20f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-10.0f, 1.0f);
            glVertex2f(-18.0f, 6.0f);
            glVertex2f(-15.0f, -2.0f);
        glEnd();

        glPushMatrix();
        glTranslatef(0.0f, 4.0f, 0.0f);
        glRotatef(flap, 0.0f, 0.0f, 1.0f);
        glColor3f(0.30f, 0.30f, 0.35f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-2.0f, 0.0f);
            glVertex2f(12.0f, 6.0f);
            glVertex2f(4.0f, -2.0f);
        glEnd();
        glPopMatrix();

        glPopMatrix();
    }
}


// ============================================================================
// STRUCTURES
// ============================================================================

void drawWindmill(float x, float y) {
    glColor3f(0.5f, 0.3f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x - 18, y);
    glVertex2f(x + 18, y);
    glVertex2f(x + 18, y + 120);
    glVertex2f(x - 18, y + 120);
    glEnd();

    glColor3f(0.6f, 0.4f, 0.3f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 35, y + 120);
    glVertex2f(x + 35, y + 120);
    glVertex2f(x, y + 160);
    glEnd();

    glPushMatrix();
    glTranslatef(x, y + 140, 0);
    glRotatef(windmillAngle * windIntensity, 0, 0, 1);

    glColor3f(0.9f, 0.9f, 0.8f);
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(90.0f * i, 0, 0, 1);
        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0);
        glVertex2f(0, 80);
        glVertex2f(12, 80);
        glEnd();
        glPopMatrix();
    }

    glPopMatrix();
}



// ============================================================================
// NEW EXTRA DETAILS
// ============================================================================

// River-side dock + fisherman with reflection
void drawDockAndFisherman() {
    float dockX = 340.0f;
    float dockY = 172.0f;

    // Dock reflection in water
    float waterMid = 150.0f;
    float refY     = waterMid - ((dockY + 5.0f) - waterMid);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.10f, 0.25f, 0.40f);
    drawEllipse(dockX, refY, 70.0f, 6.5f, 40);
    glDisable(GL_BLEND);

    // ----------------- WOODEN DOCK -----------------
    glColor3f(0.55f, 0.38f, 0.22f);
    glBegin(GL_QUADS);
        glVertex2f(dockX - 60, dockY);
        glVertex2f(dockX + 60, dockY);
        glVertex2f(dockX + 60, dockY + 10);
        glVertex2f(dockX - 60, dockY + 10);
    glEnd();

    glColor3f(0.42f, 0.28f, 0.16f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        for (float x = dockX - 58; x <= dockX + 58; x += 8.0f) {
            glVertex2f(x, dockY);
            glVertex2f(x, dockY + 10);
        }
    glEnd();

    glColor3f(0.40f, 0.26f, 0.14f);
    for (int i = -2; i <= 2; ++i) {
        float px = dockX + i * 25.0f;
        glBegin(GL_QUADS);
            glVertex2f(px - 3, 150.0f);
            glVertex2f(px + 3, 150.0f);
            glVertex2f(px + 3, dockY);
            glVertex2f(px - 3, dockY);
        glEnd();
    }

    // ----------------- SMALL BOAT TIED TO DOCK -----------------
    float boatBaseX = dockX + 50.0f;
    float boatBaseY = 150.0f;

    glColor3f(0.32f, 0.20f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(boatBaseX - 24, boatBaseY);
        glVertex2f(boatBaseX + 24, boatBaseY);
        glVertex2f(boatBaseX + 18, boatBaseY + 10);
        glVertex2f(boatBaseX - 18, boatBaseY + 10);
    glEnd();

    glColor3f(0.40f, 0.26f, 0.14f);
    glBegin(GL_LINES);
        glVertex2f(boatBaseX, boatBaseY + 10);
        glVertex2f(dockX + 40, dockY + 5);
    glEnd();

    // ----------------- FISHERMAN -----------------
    float fx = dockX - 30.0f;
    float fy = dockY + 10.0f;

    drawShadowEllipse(fx, fy - 6.0f, 10.0f, 3.5f, 0.35f);

    glColor3f(1.0f, 0.90f, 0.80f);
    drawCircle(fx, fy + 22.0f, 7.0f);

    glColor3f(0.15f, 0.35f, 0.75f);
    glBegin(GL_TRIANGLES);
        glVertex2f(fx - 7, fy + 24);
        glVertex2f(fx + 7, fy + 24);
        glVertex2f(fx,     fy + 32);
    glEnd();

    glColor3f(0.1f, 0.4f, 0.9f);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
        glVertex2f(fx, fy + 16.0f);
        glVertex2f(fx, fy);        // torso
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(fx, fy + 12.0f);
        glVertex2f(fx + 14.0f, fy + 4.0f);
        glVertex2f(fx, fy + 12.0f);
        glVertex2f(fx - 12.0f, fy + 4.0f);
    glEnd();

    glColor3f(0.1f, 0.2f, 0.35f);
    glBegin(GL_LINES);
        glVertex2f(fx, fy);
        glVertex2f(fx - 6.0f, fy - 12.0f);
        glVertex2f(fx, fy);
        glVertex2f(fx + 6.0f, fy - 12.0f);
    glEnd();

    // Fishing rod + line
    glColor3f(0.25f, 0.15f, 0.05f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(fx + 14.0f, fy + 4.0f);
        glVertex2f(fx + 70.0f, fy + 40.0f);
    glEnd();

    float bobPhase = std::sin(riverWave * 0.15f) * 4.0f;
    float lineX    = fx + 70.0f;
    float lineYTop = fy + 40.0f;
    float bobX     = lineX + 10.0f;
    float bobY     = 150.0f + bobPhase;

    glColor3f(0.95f, 0.95f, 1.0f);
    glPointSize(2.0f);
    drawLineBresenham((int)lineX, (int)lineYTop, (int)bobX, (int)bobY);


    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(bobX, bobY, 4.5f);
    glColor3f(1.0f, 0.15f, 0.15f);
    drawCircle(bobX, bobY, 3.0f);
}

// ----------------- COW HELPER -----------------
void drawSingleCow(float cx, float cy, float scale, bool faceRight) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);

    if (faceRight)
        glScalef(scale, scale, 1.0f);
    else
        glScalef(-scale, scale, 1.0f);

    float t         = personPosition * 0.08f;
    float step      = std::sin(t) * 2.5f;
    float tailSwing = std::sin(t * 1.3f) * 3.0f;

    drawShadowEllipse(0.0f, -20.0f, 26.0f, 7.0f, 0.30f);

    glColor3f(0.95f, 0.90f, 0.80f);
    glBegin(GL_QUADS);
        glVertex2f(-32, -2);
        glVertex2f( 32, -2);
        glVertex2f( 32, 18);
        glVertex2f(-32, 18);
    glEnd();
    drawEllipse(-32, 8, 8, 10, 24);
    drawEllipse( 32, 8, 8, 10, 24);

    glColor3f(0.20f, 0.20f, 0.20f);
    drawCircle(-10, 12, 4, 16);
    drawCircle(  6,  8, 3, 16);
    drawCircle( 16, 14, 3, 16);

    glColor3f(0.95f, 0.90f, 0.80f);
    drawEllipse(40, 10, 9, 7, 24);

    glColor3f(0.90f, 0.80f, 0.70f);
    glBegin(GL_QUADS);
        glVertex2f(44,  4);
        glVertex2f(54,  4);
        glVertex2f(54, 11);
        glVertex2f(44, 11);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(39, 12, 1.3f, 12);
    drawCircle(43, 12, 1.3f, 12);

    glBegin(GL_LINES);
        glVertex2f(36, 18);
        glVertex2f(32, 24);
        glVertex2f(44, 18);
        glVertex2f(48, 24);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(34, 14); glVertex2f(30, 11); glVertex2f(32, 17);
        glVertex2f(46, 14); glVertex2f(50, 11); glVertex2f(48, 17);
    glEnd();

    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(-20, -2); glVertex2f(-20, -18 - step);
        glVertex2f( -5, -2); glVertex2f( -5, -18 + step);
        glVertex2f( 10, -2); glVertex2f( 10, -18 - step);
        glVertex2f( 25, -2); glVertex2f( 25, -18 + step);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(-32, 16);
        glVertex2f(-40, 22 + tailSwing);
    glEnd();
    drawCircle(-40, 22 + tailSwing, 2.0f, 14);

    glPopMatrix();
}

// Crop field + cows + wooden fence
void drawFieldAndCow() {
    float x1 = 80.0f;
    float y1 = 40.0f;
    float x2 = 320.0f;
    float y2 = 110.0f;

    glColor3f(0.20f, 0.45f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    glColor3f(0.25f, 0.60f, 0.18f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (float x = x1 + 6; x < x2; x += 8) {
        glVertex2f(x, y1);
        glVertex2f(x, y2);
    }
    glEnd();

    // Fence around field
    glColor3f(0.55f, 0.35f, 0.18f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(x1 - 6, y2 + 4);
        glVertex2f(x2 + 6, y2 + 4);
    glEnd();
    for (float x = x1 - 6; x <= x2 + 6; x += 20.0f) {
        glBegin(GL_LINES);
            glVertex2f(x, y2 + 4);
            glVertex2f(x, y2 + 18);
        glEnd();
    }

    float baseY = 90.0f;
    drawSingleCow(WIDTH - 260.0f, baseY, 1.0f, true);
    drawSingleCow(WIDTH - 320.0f, baseY - 6.0f, 0.7f, true);
}



// Hot air balloon in sky
void drawHotAirBalloon() {
    float bx = std::fmod(balloonPosition, WIDTH + 300.0f) - 150.0f;
    float by = 520.0f + 18.0f * std::sin(balloonPosition * 0.01f);

    glPushMatrix();
    glTranslatef(bx, by, 0.0f);

    glColor3f(0.95f, 0.4f, 0.4f);
    drawCircle(0.0f, 0.0f, 22.0f);

    glColor3f(0.9f, 0.8f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-12, -6);
    glVertex2f(12, -6);
    glVertex2f(0, -24);
    glEnd();

    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_LINES);
    glVertex2f(-6, -18);
    glVertex2f(-6, -30);
    glVertex2f(6, -18);
    glVertex2f(6, -30);
    glEnd();

    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-10, -30);
    glVertex2f(10, -30);
    glVertex2f(8, -40);
    glVertex2f(-8, -40);
    glEnd();

    glPopMatrix();
}

// Fireflies at night
void drawFireflies() {
    if (isDay || dayNightBlend > 0.4f) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 40; ++i) {
        float x = 120.0f + (i * 37) % (WIDTH - 240);
        float y = 290.0f + (i * 23) % 80;

        float phase = sunAngle * 10.0f + i * 0.4f;
        float a = 0.2f + 0.4f * (std::sin(phase) * 0.5f + 0.5f);

        glColor4f(1.0f, 1.0f, 0.6f, a);
        glVertex2f(x, y);
    }
    glEnd();

    glDisable(GL_BLEND);
}



// Kite in sky
void drawKite() {
    float tx = std::fmod(kitePosition, WIDTH + 200.0f) - 100.0f;
    float ty = 520.0f + 15.0f * std::sin(kitePosition * 0.03f);

    // ✅ BIG values for clear demo
    float sx  = useScaleT ? 1.8f : 1.0f;
    float sy  = useScaleT ? 1.8f : 1.0f;

    float rot = useRotateT ? (std::sin(kitePosition * 0.02f) * 60.0f) : 0.0f;

    // ✅ Reflection always ON when toggle is ON (no random flip)
    bool refX = false;
    bool refY = useReflectT;

    // ✅ BIG shear values so V is clearly visible
    float shx = useShearT ? 0.90f : 0.0f;
    float shy = useShearT ? 0.40f : 0.0f;

    // ✅ bigger diamond for better visibility
    Vec2 top    = {  0,  28 };
    Vec2 right  = { 22,   0 };
    Vec2 bottom = {  0, -28 };
    Vec2 left   = { -22,  0 };

    Vec2 T = applyAllTransforms(top,    sx, sy, rot, refX, refY, shx, shy, tx, ty);
    Vec2 R = applyAllTransforms(right,  sx, sy, rot, refX, refY, shx, shy, tx, ty);
    Vec2 B = applyAllTransforms(bottom, sx, sy, rot, refX, refY, shx, shy, tx, ty);
    Vec2 L = applyAllTransforms(left,   sx, sy, rot, refX, refY, shx, shy, tx, ty);

    // ✅ Two different colors (so Reflection C is obvious)
    // Right half (red)
    glColor3f(1.0f, 0.25f, 0.25f);
    glBegin(GL_TRIANGLES);
        glVertex2f(T.x, T.y);
        glVertex2f(R.x, R.y);
        glVertex2f(B.x, B.y);
    glEnd();

    // Left half (blue)
    glColor3f(0.25f, 0.65f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(T.x, T.y);
        glVertex2f(B.x, B.y);
        glVertex2f(L.x, L.y);
    glEnd();

    // Outline (black)
    glColor3f(0.08f, 0.08f, 0.08f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(T.x, T.y);
        glVertex2f(R.x, R.y);
        glVertex2f(B.x, B.y);
        glVertex2f(L.x, L.y);
    glEnd();

    // Cross stick (single line)
    Vec2 stickL = { -20, 0 };
    Vec2 stickR = {  20, 0 };
    stickL = applyAllTransforms(stickL, sx, sy, rot, refX, refY, shx, shy, tx, ty);
    stickR = applyAllTransforms(stickR, sx, sy, rot, refX, refY, shx, shy, tx, ty);

    glColor3f(0.95f, 0.85f, 0.25f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(stickL.x, stickL.y);
        glVertex2f(stickR.x, stickR.y);
    glEnd();

    // ✅ Asymmetric sticker on RIGHT side (Reflection moves it to LEFT)
    Vec2 s1 = {  8,  10 };
    Vec2 s2 = { 16,  10 };
    Vec2 s3 = { 16,   2 };
    Vec2 s4 = {  8,   2 };

    s1 = applyAllTransforms(s1, sx, sy, rot, refX, refY, shx, shy, tx, ty);
    s2 = applyAllTransforms(s2, sx, sy, rot, refX, refY, shx, shy, tx, ty);
    s3 = applyAllTransforms(s3, sx, sy, rot, refX, refY, shx, shy, tx, ty);
    s4 = applyAllTransforms(s4, sx, sy, rot, refX, refY, shx, shy, tx, ty);

    glColor3f(0.10f, 0.95f, 0.30f); // green
    glBegin(GL_QUADS);
        glVertex2f(s1.x, s1.y);
        glVertex2f(s2.x, s2.y);
        glVertex2f(s3.x, s3.y);
        glVertex2f(s4.x, s4.y);
    glEnd();

    // ✅ Tail (offset) -> reflection very clear
    Vec2 tailA = { 10, -28 };
    Vec2 tailB = { 20, -75 };

    tailA = applyAllTransforms(tailA, sx, sy, rot, refX, refY, shx, shy, tx, ty);
    tailB = applyAllTransforms(tailB, sx, sy, rot, refX, refY, shx, shy, tx, ty);

    glColor3f(0.95f, 0.85f, 0.25f);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glVertex2f(tailA.x, tailA.y);
        glVertex2f(tailB.x, tailB.y);
    glEnd();
}





// Well near a house
void drawWell(float x, float y) {
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(x - 18, y);
        glVertex2f(x + 18, y);
        glVertex2f(x + 18, y + 25);
        glVertex2f(x - 18, y + 25);
    glEnd();

    glColor3f(0.4f, 0.4f, 0.4f);
    drawEllipse(x, y + 25, 18.0f, 6.0f, 24);
    glColor3f(0.2f, 0.2f, 0.2f);
    drawEllipse(x, y + 22, 14.0f, 4.0f, 24);

    glColor3f(0.35f, 0.25f, 0.15f);
    glBegin(GL_LINES);
        glVertex2f(x - 16, y + 25);
        glVertex2f(x - 16, y + 55);
        glVertex2f(x + 16, y + 25);
        glVertex2f(x + 16, y + 55);
        glVertex2f(x - 16, y + 55);
        glVertex2f(x + 16, y + 55);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(x, y + 55);
        glVertex2f(x, y + 35);
    glEnd();

    drawCircle(x, y + 33, 2.0f);
}

// ============================================================================
// WEATHER EFFECTS
// ============================================================================
void drawRain() {
    if (!isRaining) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // wind slant
    float slant = 10.0f * windIntensity;   // increase with wind

    // -------- FAR LAYER (thin, faint) --------
    glColor4f(0.80f, 0.85f, 1.0f, 0.22f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 260; i++) {
        float x = (i * 37) % WIDTH;
        float y = std::fmod((float)(i * 61) - rainOffset * 0.7f + HEIGHT, (float)HEIGHT);
        glVertex2f(x, y);
        glVertex2f(x + slant * 0.6f, y - 14.0f);
    }
    glEnd();

    // -------- MID LAYER --------
    glColor4f(0.85f, 0.88f, 1.0f, 0.45f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for (int i = 0; i < 320; i++) {
        float x = (i * 29) % WIDTH;
        float y = std::fmod((float)(i * 47) - rainOffset + HEIGHT, (float)HEIGHT);

        float len = 18.0f + (i % 5) * 2.5f;     // different length
        glVertex2f(x, y);
        glVertex2f(x + slant * 0.8f, y - len);
    }
    glEnd();

    // -------- NEAR LAYER (bright, thick) --------
    glColor4f(0.90f, 0.90f, 1.0f, 0.70f);
    glLineWidth(2.2f);
    glBegin(GL_LINES);
    for (int i = 0; i < 180; i++) {
        float x = (i * 53 + 120) % WIDTH;
        float y = std::fmod((float)(i * 83) - rainOffset * 1.3f + HEIGHT, (float)HEIGHT);

        float len = 26.0f + (i % 4) * 4.0f;
        glVertex2f(x, y);
        glVertex2f(x + slant, y - len);
    }
    glEnd();

    // -------- SPLASH (near ground & river line) --------
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 120; i++) {
        float x = (i * 41) % WIDTH;

        // splash on ground area (around y=200..260) and river edges (110..190)
        float y;
        if (i % 2 == 0) y = 200.0f + (i % 25);   // footpath/road zone
        else           y = 118.0f + (i % 60);    // river zone

        glColor4f(0.90f, 0.95f, 1.0f, 0.45f);
        glVertex2f(x, y);
        glVertex2f(x + 2, y + 1);
    }
    glEnd();

    glDisable(GL_BLEND);
}

// ============================================================================
// PEOPLE - WALKING 
// ============================================================================

void drawWalkingPerson() {
    float px   = std::fmod(personPosition, WIDTH + 200.0f) - 100.0f;
    float base = 225.0f;
    float py   = base + 4.0f * std::sin(personPosition * 0.15f);
    float step = std::sin(personPosition * 0.3f) * 6.0f;

    drawShadowEllipse(px, base - 10.0f, 11.0f, 3.5f, 0.35f);

    glColor3f(1.0f, 0.9f, 0.8f);
    drawCircle(px, py + 22.0f, 7.0f);

    glColor3f(0.8f, 0.2f, 0.2f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(px, py + 16.0f);
    glVertex2f(px, py);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(px, py + 12.0f);
    glVertex2f(px - 8.0f - step * 0.4f, py + 4.0f);
    glVertex2f(px, py + 12.0f);
    glVertex2f(px + 8.0f + step * 0.4f, py + 4.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(px, py);
    glVertex2f(px - 6.0f - step, py - 14.0f);
    glVertex2f(px, py);
    glVertex2f(px + 6.0f + step, py - 14.0f);
    glEnd();
}

void initWalkers() {
    for (int i = 0; i < NUM_WALKERS; i++) {
        walkerPositions[i] = i * 150.0f;  // spread them out
    }
}

void updateWalkers() {
    for (int i = 0; i < NUM_WALKERS; i++) {
        walkerPositions[i] += walkerSpeeds[i];
        if (walkerPositions[i] > WIDTH + 200) walkerPositions[i] = -200;
    }
}


// ============================================================================
// SCENE COMPOSITION - FIXED LAYER ORDER
// ============================================================================

void drawVillageScene() {
    drawSky();
    drawStars();
    drawSunMoon();
    drawClouds();
    drawHotAirBalloon();
    drawKite();

    drawDistantHills();

    drawGround();
    drawFieldAndCow();

    drawRiver();
    drawFish();

    drawRailTrack();
    if (showTrain) drawMovingTrain();

    drawWindmill(950, 320);
    for (int i = 0; i < 10; i++) {
        drawTree(150 + i * 120, 320 + (i % 3) * 10, i);
    }

    // Houses row
    float houseY = 285.0f;     // ✅ same base for all houses
    float gap    = 210.0f;     // spacing

    drawModernHouse(  80, houseY);
    drawTraditionalHouse( 80 + gap, houseY);
    drawFarmHouse(  80 + 2*gap, houseY);
    drawTraditionalHouse( 80 + 3*gap, houseY);
    drawModernHouse(  80 + 4*gap, houseY);
    drawFarmHouse(  80 + 5*gap, houseY);


    // Well near second house
    drawWell(230, 260);

    drawRoad();
    drawMovingCar();
    drawDockAndFisherman();  // dock on river bank
    drawMovingBoat();        // boat is top layer on water

    if (showBirds)  drawBirds();
    if (showPlane)  drawAirplane();
    if (showPerson) {
    for (int i = 0; i < NUM_WALKERS; i++) {
        personPosition = walkerPositions[i];   // temporarily set global for drawWalkingPerson
        drawWalkingPerson();
    }
}

    drawFireflies();
    drawRain();
}

// ============================================================================
// ANIMATION UPDATE
// ============================================================================

void update(int value) {
    if (!animationPaused) {
        float speed = speedFactor;

        const float PI = 3.1415926f;

        // ✅ day-night angle (NO WRAP / NO AUTO RESET)
        sunAngle += 0.008f * speed;

        // ✅ use phase only for checking day/night (0..2PI)
        float phase = std::fmod(sunAngle, 2.0f * PI);
        if (phase < 0.0f) phase += 2.0f * PI;

        cloudOffset     += 0.4f   * speed * windIntensity;
        boatPosition += 1.2f * speed;
        carPosition     += 1.2f   * speed;
        birdOffset += 1.8f * speed;
        windmillAngle   += 2.5f   * speed * windIntensity;
        planePosition   += 2.2f   * speed;
        trainPosition   -= 1.6f   * speed;
        updateWalkers();   // update the 5 walker positions
        riverWave       += 0.5f   * speed;
        fishPosition    += 1.3f   * speed;
        smokeOffset     += 0.3f   * speed;
        balloonPosition += 0.5f   * speed;
        kitePosition    += 1.0f   * speed * windIntensity;

        // ✅ day/night decision uses phase (NOT sunAngle)
        bool prevIsDay = isDay;
        isDay = (phase < PI);
        if (isDay != prevIsDay) {
            printf("Switched to %s\n", isDay ? "Day" : "Night");
        }

        // ✅ wind uses phase too (optional but better)
        float baseWind = 0.8f + 0.4f * std::sin(phase * 0.3f);
        windIntensity = baseWind * windUser;

        swingAngle += (swingForward ? 0.4f : -0.4f) * speed;
        if (swingAngle > 20.0f)  swingForward = false;
        if (swingAngle < -20.0f) swingForward = true;

        if (isRaining) {
            rainOffset += 8.0f * speed;
            if (rainOffset > HEIGHT) rainOffset = 0.0f;
        }

        
        // Wrap positions (keep these, they are for objects only)
        if (cloudOffset > WIDTH + 300) cloudOffset = -300;
        if (boatPosition > WIDTH + 200) boatPosition = -200;
        if (birdOffset > WIDTH + 150) birdOffset = -150;
        if (carPosition > WIDTH + 250) carPosition = -250;
        if (planePosition > WIDTH + 350) planePosition = -350;
        if (trainPosition < -800)       trainPosition = WIDTH + 400;
        if (fishPosition > WIDTH + 300) fishPosition = -300;
        if (balloonPosition > WIDTH + 600) balloonPosition = 0.0f;
        if (kitePosition > WIDTH + 400)    kitePosition = 0.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


// ============================================================================
// DISPLAY AND INPUT HANDLING
// ============================================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawVillageScene();

    // HUD bar (✅ make it taller because now 3 lines)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.45f);
    glBegin(GL_QUADS);
        glVertex2f(0, HEIGHT - 80);      // ✅ was -60
        glVertex2f(WIDTH, HEIGHT - 80);  // ✅ was -60
        glVertex2f(WIDTH, HEIGHT);
        glVertex2f(0, HEIGHT);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(1.0f, 1.0f, 1.0f);

    char info[350];

    // ---------- Line 1 ----------
    glRasterPos2f(10, HEIGHT - 20);
    if (animationPaused) {
        sprintf(info,
            "VILLAGE (PAUSED) | P: Play | D/N: Day/Night | "
            "R: Rain | 1/2: Speed | E: Reset | W/S: Wind");
    } else {
        sprintf(info,
            "VILLAGE (RUNNING) | P: Pause | D/N: Day/Night | "
            "R: Rain | 1/2: Speed | E: Reset | W/S: Wind");
    }
    for (int i = 0; info[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);

    // ---------- Line 2 ----------
    glRasterPos2f(10, HEIGHT - 38);
    sprintf(info,
        "B: Birds %s | A: Plane %s | G: Train %s | H: Person %s | ESC: Exit",
        showBirds ? "ON" : "OFF",
        showPlane ? "ON" : "OFF",
        showTrain ? "ON" : "OFF",
        showPerson ? "ON" : "OFF");
    for (int i = 0; info[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);

    // ---------- Line 3 (✅ NEW: Z/X/C/V) ----------
    glRasterPos2f(10, HEIGHT - 56);
    sprintf(info,
        "Transforms: Z(Scale) %s | X(Rotate) %s | C(Reflect) %s | V(Shear) %s",
        useScaleT   ? "ON" : "OFF",
        useRotateT  ? "ON" : "OFF",
        useReflectT ? "ON" : "OFF",
        useShearT   ? "ON" : "OFF");
    for (int i = 0; info[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);

    // ---------- Right side status ----------
    glRasterPos2f(WIDTH - 360, HEIGHT - 20);
    sprintf(info, "MODE: %s | Rain: %s | Speed: %.1fx",
        isDay ? "DAY" : "NIGHT",
        isRaining ? "ON" : "OFF",
        speedFactor);
    for (int i = 0; info[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);

    glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'p': case 'P':
            animationPaused = !animationPaused;
            printf("Animation %s\n", animationPaused ? "Paused" : "Resumed");
            break;

        case 'd': case 'D':
            sunAngle      = 0.3f;
            isDay         = true;
            dayNightBlend = 1.0f;
            printf("Switched to Day mode\n");
            break;

        case 'n': case 'N':
            sunAngle      = 3.4f;
            isDay         = false;
            dayNightBlend = 0.0f;
            printf("Switched to Night mode\n");
            break;


        case '1':
            speedFactor *= 1.3f;
            if (speedFactor > 8.0f) speedFactor = 8.0f;
            printf("Speed increased: %.2fx\n", speedFactor);
            break;

        case '2':
            speedFactor *= 0.7f;
            if (speedFactor < 0.1f) speedFactor = 0.1f;
            printf("Speed decreased: %.2fx\n", speedFactor);
            break;

        case 'w': case 'W':
            windUser += 0.2f;
            if (windUser > 2.0f) windUser = 2.0f;
            printf("Wind increased: %.1f\n", windUser);
            break;

        case 's': case 'S':
            windUser -= 0.2f;
            if (windUser < 0.2f) windUser = 0.2f;
            printf("Wind decreased: %.1f\n", windUser);
            break;

        case 'b': case 'B':
            showBirds = !showBirds;
            printf("Birds %s\n", showBirds ? "ON" : "OFF");
            break;

        case 'a': case 'A':
            showPlane = !showPlane;
            printf("Airplane %s\n", showPlane ? "ON" : "OFF");
            break;

        case 'g': case 'G':
            showTrain = !showTrain;
            printf("Train %s\n", showTrain ? "ON" : "OFF");
            break;


        case 'h': case 'H':
            showPerson = !showPerson;
            printf("Person %s\n", showPerson ? "ON" : "OFF");
            break;


        // ✅ R = Rain toggle
        case 'r': case 'R':
            isRaining = !isRaining;
            printf("Rain %s\n", isRaining ? "ON" : "OFF");
            break;

        case 'z': case 'Z':
            useScaleT = !useScaleT;
            printf("Algorithm Scale %s\n", useScaleT ? "ON" : "OFF");
            break;

        case 'x': case 'X':
            useRotateT = !useRotateT;
            printf("Algorithm Rotate %s\n", useRotateT ? "ON" : "OFF");
            break;

        case 'c': case 'C':
            useReflectT = !useReflectT;
            printf("Algorithm Reflection %s\n", useReflectT ? "ON" : "OFF");
            break;

        case 'v': case 'V':
            useShearT = !useShearT;
            printf("Algorithm Shear %s\n", useShearT ? "ON" : "OFF");
            break;

        // ✅ RESET moved to E (because R is rain now)
        case 'e': case 'E':
            sunAngle        = 0.0f;
            cloudOffset     = 0.0f;
            boatPosition    = 0.0f;
            birdOffset      = 0.0f;
            windmillAngle = 0.0f;
            carPosition     = 0.0f;
            swingAngle = 0.0f;
            swingForward    = true;
            planePosition   = 0.0f;
            rainOffset      = 0.0f;
            speedFactor     = 1.0f;
            trainPosition   = WIDTH + 400.0f;
            personPosition  = 0.0f;
            riverWave       = 0.0f;
            fishPosition    = 0.0f;
            smokeOffset     = 0.0f;
            windIntensity   = 1.0f;
            dayNightBlend   = isDay ? 1.0f : 0.0f;
            balloonPosition = 0.0f;
            kitePosition    = 0.0f;

            showBirds       = true;
            showPlane       = true;
            showTrain       = true;
            showPerson      = true;

            // also reset transform toggles
            useScaleT   = true;
            useRotateT  = true;
            useReflectT = true;
            useShearT   = true;

            printf("All animations & toggles reset (E)\n");
            break;

        case 27:
            printf("Exiting program\n");
            exit(0);
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        printf("Mouse clicked at: (%d, %d)\n", x, HEIGHT - y);
        animationPaused = !animationPaused;
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Advanced Realistic Village with River Boat - Final Version v4");

    printf("==================================================================\n");
    printf("VILLAGE Scenaro\n");
    printf("New extras: Sun & moon from horizon\n");
    printf("dock + fisherman with reflection, crop field + fence + cows,\n");
    printf("hot-air balloon, kite\n");
    printf("fireflies\n");
    printf("Controls:\n");
    printf("  P: Pause/Play   D/N: Force Day/Night   R: Rain ON/OFF\n");
    printf("  1/2: Speed +/-  W/S: Wind +/-  \n");
    printf("  B: Birds   A: Airplane   G: Train   L: Light glow\n");
    printf("  H: Person  E: Reset   ESC: Exit\n");
    printf("==================================================================\n");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, update, 0);

    initRendering();
    initWalkers();

    glutMainLoop();
    return 0;
}
