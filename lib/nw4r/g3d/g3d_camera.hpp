// Source:
// https://github.com/doldecomp/ogws/blob/82dbeac2267170fc5cef7e67a6d8c31c5ff45d69/include/nw4r/g3d/g3d_camera.h
// Decompiled by kiwi515.

#pragma once

#include <rk_types.h>

#include <nw4r/math/mathTypes.hpp>
#include <rvl/mtx/mtx.h>

#include "g3d_rescommon.hpp"

namespace nw4r {
namespace g3d {
struct CameraData {
  math::MTX34 mCamMtx;  // at 0x0
  math::MTX44 mProjMtx; // at 0x30
  u32 mFlags;
  math::VEC3 mPos; // at 0x74
  math::VEC3 VEC3_0x80;
  math::VEC3 VEC3_0x8C;
  math::VEC3 VEC3_0x98;
  f32 FLOAT_0xA4;
  u32 mIsOrtho;
  f32 mFovY;
  f32 mAspectRatio;
  f32 mNearPlane;
  f32 mFarPlane;
  f32 mTop;
  f32 mBottom;
  f32 mLeft;
  f32 mRight;
  f32 FLOAT_0xCC;
  f32 FLOAT_0xD0;
  f32 FLOAT_0xD4;
  f32 FLOAT_0xD8;
  f32 mViewportX;
  f32 mViewportY;
  f32 mViewportW;
  f32 mViewportH;
  f32 mViewportZStart;
  f32 mViewportZEnd;
  u32 mScissorX;
  u32 mScissorY;
  u32 mScissorW;
  u32 mScissorH;
  u32 mScissorBoxOffsX;
  u32 mScissorBoxOffsY;
};

struct Camera {
  struct PostureInfo {
    int INT_0x0;
    math::VEC3 VEC3_0x04;
    math::VEC3 VEC3_0x10;
    math::VEC3 VEC3_0x1C;
    f32 FLOAT_0x28;
    // . . .
  };

  ResCommon<CameraData> mCamData;

  inline Camera(void* vptr) : mCamData(vptr) {}
  inline void UpdateProjectionMtx() const {
    CameraData& rCamData = mCamData.ref();

    if (rCamData.mFlags & 0x40) {
      C_MTXOrtho(rCamData.mProjMtx, rCamData.mTop, rCamData.mBottom,
                 rCamData.mLeft, rCamData.mRight, rCamData.mNearPlane,
                 rCamData.mFarPlane);
    } else {
      if (rCamData.mFlags & 0x10) {
        C_MTXFrustum(rCamData.mProjMtx, rCamData.mTop, rCamData.mBottom,
                     rCamData.mLeft, rCamData.mRight, rCamData.mNearPlane,
                     rCamData.mFarPlane);
      } else {
        C_MTXPerspective(rCamData.mProjMtx, rCamData.mFovY,
                         rCamData.mAspectRatio, rCamData.mNearPlane,
                         rCamData.mFarPlane);
      }
    }

    rCamData.mFlags |= 0x80;
  }

  // PAL: 0x8006a0c0
  Camera(CameraData*);
  // PAL: 0x8006a0d0
  void Init();
  // PAL: 0x8006a120
  void Init(u16, u16, u16, u16, u16, u16);
  // PAL: 0x8006a240
  void SetPosition(f32, f32, f32);
  // PAL: 0x8006a270
  void SetPosition(const math::VEC3&);
  // PAL: 0x8006a2b0
  void SetPosture(const PostureInfo&);
  // PAL: 0x8006a4d0
  void SetCameraMtxDirectly(const math::MTX34&);
  // PAL: 0x8006a520
  void SetPerspective(f32, f32, f32, f32);
  // PAL: 0x8006a560
  void SetOrtho(f32, f32, f32, f32, f32, f32);
  // PAL: 0x8006a5a0
  void SetProjectionMtxDirectly(const math::MTX44*);
  // PAL: 0x8006a5f0
  void SetScissor(u32, u32, u32, u32);
  // PAL: 0x8006a610
  void SetScissorBoxOffset(s32, s32);
  // PAL: 0x8006a630
  void SetViewport(f32, f32, f32, f32);
  // PAL: 0x8006a6d0
  void SetViewportZRange(f32, f32);
  // PAL: 0x8006a6f0
  void GetViewport(f32*, f32*, f32*, f32*, f32*, f32*) const;
  // PAL: 0x8006a760
  void GetCameraMtx(math::MTX34*) const;
  // PAL: 0x8006a7c0
  void GetProjectionMtx(math::MTX44*) const;
  // PAL: 0x8006a8a0
  void GetProjectionTexMtx(math::MTX34*) const;
  // PAL: 0x8006a970
  void GetEnvironmentTexMtx(math::MTX34*) const;
  // PAL: 0x8006a9f0
  void GXSetViewport_() const;
  // PAL: 0x8006aa80
  void GXSetProjection_() const;
  // PAL: 0x8006ab50
  void GXSetScissor_() const;
  // PAL: 0x8006ab80
  void GXSetScissorBoxOffset_() const;
  // PAL: 0x8006aba0
  void UpdateCameraMtx() const;
};
} // namespace g3d
} // namespace nw4r
