#include "Application/Application.h"
#include "src/tests/New_Paint_Test/PaintLayer.h"

int main() {
    Application app(1280, 720, "SCED Paint");
    PaintLayer paint(app.getRenderer(), app.getShader(), app.getInput(), app.getWindow());
    app.setLayer(&paint);
    app.run();
    return 0;
}
