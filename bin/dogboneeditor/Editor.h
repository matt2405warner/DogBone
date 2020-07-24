//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_EDITOR_H
#define DOGBONE_EDITOR_H

#include <UT/UT_Application.h>

#include <memory>

namespace dogb
{
class Editor : public dogb::UT::Application
{
public:
    explicit Editor (const std::string& name) :
        UT::Application(name)
    {}
    ~Editor () override = default;

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

#endif // DOGBONE_EDITOR_H
