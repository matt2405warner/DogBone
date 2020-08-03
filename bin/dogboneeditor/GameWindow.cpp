//
// Created by matt on 2020-07-11.
//

#include "GameWindow.h"
namespace dogb
{
void
GameWindow::onStart()
{
    m_title = "Game";

    m_dockDirection = DockNone;
}

void
GameWindow::onGUI(const UT::Timestep&)
{
}

} // namespace dogb