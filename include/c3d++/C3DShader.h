#pragma once

#include <3ds/gpu/shaderProgram.h>
#include <3ds/types.h>
#include <memory>
#include <span>

class C3DShader {
public:
    C3DShader();
    C3DShader(const u8* shbin, u32 size);

    shaderProgram_s& getShaderProgram();

    bool valid();

    ~C3DShader();

private:
    bool valid_ = false;
    u8 const* shbin_;
    u32 size_;
    shaderProgram_s program_;
    DVLB_s* shader_dvlb_;
};