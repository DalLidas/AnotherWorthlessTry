#include "Engine.h"

BOOL Engine::Initialize(HINSTANCE hInstance, std::string windowName, std::string windowClass, INT width, INT height)
{
    timer.Start();

    //keyboard.EnableAutoRepeatChars();
    keyboard.DisableAutoRepeatChars();

    //keyboard.EnableAutoRepeatKeys();
    keyboard.DisableAutoRepeatKeys();

    if (!this->window.Initialize(this, hInstance, windowName, windowClass, width, height)) {
        return false;
    }

    if (!gfx.Initialize(window.GetHWND(), width, height)) {
        return false;
    }

    physics.Initialize(scene.GetSceneBorder());

    return true;

}

BOOL Engine::ProcessMessages()
{
    return  this->window.ProcessMessages();
}

void Engine::Update(BindMSG& imGuiMsg_)
{
    float dt = static_cast<float>(timer.GetMilisecondsElapsed());
    timer.Restart();

#ifdef INPUT_DEBUG_MSG
    while (!keyboard.CharBufferIsEmpty())
    {
        unsigned char ch = keyboard.ReadChar();
    }


    while (!keyboard.KeyBufferIsEmpty())
    {
        KeyboardEvent kbe = keyboard.ReadKey();
        unsigned char keycode = kbe.GetKeyCode();
    }
#endif //INPUT_DEBUG_MSG

    while (!mouse.EventBufferIsEmpty())
    {
        MouseEvent me = mouse.ReadEvent();
        if (mouse.IsRightDown())
        {
            if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
            {
                this->gfx.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
            }
        }
    }

    const float cameraSpeed = 0.01f;

    if (keyboard.KeyIsPressed('W'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('S'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('A'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('D'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed(VK_SPACE))
    {
        this->gfx.camera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
    }
    if (keyboard.KeyIsPressed(VK_SHIFT))
    {
        this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
    }

    //ReadImGuiMsg
    scene.SetSceneBorder(imGuiMsg_.sceneBorder);
    scene.SetPointOfGod(imGuiMsg_.pointOfGod);
    physics.SetStates(imGuiMsg_.gAccelerationState, imGuiMsg_.bounceDicrimentState, imGuiMsg_.airResistanceState);
    physics.SetBounceDicrement(imGuiMsg_.bounceDicrement);
    physics.SetAirResistanceDicrement(imGuiMsg_.airResistanceDicrement);
    float movMultiplier = 0.001f * imGuiMsg_.movMultiplier;
    float accMultiplier = 0.001f * imGuiMsg_.accMultiplier;
    float CreatedAccMultiplier = imGuiMsg_.CreatedAccMultiplier;

    //Scene events
    if (imGuiMsg_.createRandomState) {
        scene.CreatePoint(
            1.0f,
            scene.GetPointOfGod(),
            DirectX::XMFLOAT3{ (rand() % 100 - 50) / 1000.0f, (rand() % 100 - 50) / 1000.0f, (rand() % 100 - 50) / 1000.0f },
            DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f }
        );
    }

    if (imGuiMsg_.createState) {
        scene.CreatePoint(
            imGuiMsg_.point.radius,
            scene.GetPointOfGod(),
            imGuiMsg_.point.velosity,
            imGuiMsg_.point.acceleration,
            imGuiMsg_.point.moveState,
            imGuiMsg_.point.createGravityState
        );
    }

    if (imGuiMsg_.destroyState) {
        scene.DestroyAllPoint();
    }

    //Physics events
    physics.SetDeltaTime(dt);
    physics.SetSceneBorder(scene.GetSceneBorder());

    if (imGuiMsg_.phsicsState) {
        for (size_t i = 0; i < this->scene.GetPoints().size(); ++i) {
            if (this->scene.GetPoints().at(i).moveState) {
                Point bufPoint{};
                DirectX::XMFLOAT3 resultBufAcc{0, 0, 0};
                int colideBorderSide = 0;
                bufPoint = physics.Move(this->scene.GetPoints().at(i), movMultiplier);

                colideBorderSide = physics.BorderCollision(bufPoint);
                if (colideBorderSide != 0) {
                    bufPoint = physics.BounceFromBorder(bufPoint, colideBorderSide);
                }
                else {
                    for (size_t j = 0; j < this->scene.GetPoints().size(); ++j) {
                        DirectX::XMFLOAT3 bufAcc{};
                        if (i != j) {
                            if (physics.ObjectCollision(bufPoint, this->scene.GetPoints().at(j))) {
                                std::pair<Point, Point> bufPointPair{};
                                bufPointPair = physics.BounceFromObject(this->scene.GetPoints().at(i), this->scene.GetPoints().at(j));

                                bufPoint = physics.Move(bufPointPair.first, movMultiplier);
                                if (!this->scene.GetPoints().at(i).moveState) this->scene.SetPoint(physics.Move(bufPointPair.second, movMultiplier), j);
                            }
                            if (this->scene.GetPoints().at(j).createGravityState) {
                                bufAcc = physics.CalcAcceleration(this->scene.GetPoints().at(i), this->scene.GetPoints().at(j), CreatedAccMultiplier);
                                resultBufAcc.x += bufAcc.x;
                                resultBufAcc.y += bufAcc.y;
                                resultBufAcc.z += bufAcc.z;
                            }
                        }
                    }

                }

                bufPoint.acceleration.x = this->scene.GetPoints().at(i).acceleration.x + resultBufAcc.x;
                bufPoint.acceleration.y = this->scene.GetPoints().at(i).acceleration.y + resultBufAcc.y;
                bufPoint.acceleration.z = this->scene.GetPoints().at(i).acceleration.z + resultBufAcc.z;

                this->scene.SetPoint(bufPoint, i);
                this->scene.SetPoint(physics.Accelerate(bufPoint, accMultiplier), i);

            }
        }

    }

#ifdef INPUT_DEBUG_MSG
    while (!keyboard.CharBufferIsEmpty()) {
        unsigned char ch = keyboard.ReadChar();
        std::string outmsg = "Char: ";
        outmsg += ch;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }

    while (!keyboard.KeyBufferIsEmpty()) {

        KeyboardEvent kbe = keyboard.ReadKey();

        unsigned char keyCode = kbe.GetKeyCode();

        std::string outmsg = "";
        if (kbe.IsPress()) {
            outmsg += "Key pressed: ";
        }
        if (kbe.IsRealize()) {
            outmsg += "Key realized: ";
        }

        outmsg += keyCode;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }

    /*
    while (!mouse.EventBufferIsEmpty()) {
        MouseEvent me = mouse.ReadEvent();

        std::string outmsg = "";
        outmsg += "(X:" + std::to_string(me.GetPosX()) + "||";
        outmsg += "Y:" + std::to_string(me.GetPosX()) + ")\n";

        OutputDebugStringA(outmsg.c_str());
    }
    */

    while (!mouse.EventBufferIsEmpty()) {
        MouseEvent me = mouse.ReadEvent();

        if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
            std::string outmsg = "";
            outmsg += "(X:" + std::to_string(me.GetPosX()) + "||";
            outmsg += "Y:" + std::to_string(me.GetPosX()) + ")\n";

            OutputDebugStringA(outmsg.c_str());
        }

    }
    )
#endif // INPUT_DEBUG_MSG
}

void Engine::RenderFrame(BindMSG& imGuiMsg_)
{
    gfx.RenderFrame(scene.GetPoints(), imGuiMsg_);
}
