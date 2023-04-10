#include <3ds.h>
#include <c3d++/C3DShader.h>
#include <citro3d.h>

C3DShader::C3DShader()
{
}

C3DShader::C3DShader(const u8* shbin, u32 size)
{
    shbin_ = shbin;
    size_ = size;
    shader_dvlb_ = DVLB_ParseFile((u32*)shbin, size);
    shaderProgramInit(&program_);
    shaderProgramSetVsh(&program_, &shader_dvlb_->DVLE[0]);
    valid_ = true;
}

C3DShader::~C3DShader()
{
    DVLB_Free(shader_dvlb_);
    shader_dvlb_ = nullptr;
    shaderProgramFree(&program_);
    valid_ = false;
}

shaderProgram_s& C3DShader::getShaderProgram()
{
    return program_;
}

bool C3DShader::valid()
{
    return valid_;
}