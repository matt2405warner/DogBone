//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_EDITORAPP_H
#define DOGBONE_EDITORAPP_H

#include <GS/GS_Entity.h>

#include <UT/UT_Application.h>

#include <memory>

namespace dogb
{
class EditorApp : public dogb::UT::Application
{
public:
    explicit EditorApp(const std::string &name) : UT::Application(name) {}
    ~EditorApp() override = default;

protected:
    int initialize(int argc, char* argv[]) override;
    int exec() override;
    int cleanup() override ;
};

class EditorInterface : public dogb::UT::AppInterface
{
protected:
    std::unique_ptr<dogb::UT::Application> create() override;
};

} // namespace dogb

#endif // DOGBONE_EDITORAPP_H
