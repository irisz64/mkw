// Source:
// https://github.com/doldecomp/ogws/blob/82dbeac2267170fc5cef7e67a6d8c31c5ff45d69/src/nw4r/g3d/g3d_camera.cpp
// Decompiled by kiwi515.

#include "g3d_camera.hpp"

#include <decomp.h>

#include <rvl/gx/gxTransform.h>
#include <rvl/mtx/mtx.h>

#include "g3d_state.hpp"

namespace nw4r {
namespace g3d {

Camera::Camera(CameraData* pCamData) : mCamData(pCamData) {}

void Camera::Init() {
  GXRenderModeObj& rRenderMode = G3DState::GetRenderModeObj();
  Init(rRenderMode.fb_width, rRenderMode.efb_height, rRenderMode.fb_width,
       rRenderMode.xfb_height, rRenderMode.vi_width, rRenderMode.vi_height);
}

void Camera::Init(u16 scissorWidth, u16 scissorHeight, u16 viewportW,
                  u16 viewportH, u16 top, u16 bottom) {
  CameraData& rCamData = mCamData.ref();

  if (mCamData.IsValid()) {
    rCamData.mFlags = 0x21;

    rCamData.mPos.x = 0.0f;
    rCamData.mPos.y = 0.0f;
    rCamData.mPos.z = 15.0f;

    rCamData.VEC3_0x80.x = 0.0f;
    rCamData.VEC3_0x80.y = 1.0f;
    rCamData.VEC3_0x80.z = 0.0f;

    rCamData.VEC3_0x8C.x = 0.0f;
    rCamData.VEC3_0x8C.y = 0.0f;
    rCamData.VEC3_0x8C.z = 0.0f;

    rCamData.VEC3_0x98.x = 0.0f;
    rCamData.VEC3_0x98.y = 0.0f;
    rCamData.VEC3_0x98.z = 0.0f;

    rCamData.FLOAT_0xA4 = 0.0f;
    rCamData.mIsOrtho = 0;
    rCamData.mFovY = 60.0f;
    rCamData.mAspectRatio = (4.0f / 3.0f);
    rCamData.mNearPlane = 0.1f;
    rCamData.mFarPlane = 1000.f;
    rCamData.mTop = 0.0f;
    rCamData.mBottom = (float)bottom;
    rCamData.mLeft = 0.0f;
    rCamData.mRight = (float)top;
    rCamData.FLOAT_0xCC = 0.5f;
    rCamData.FLOAT_0xD0 = 0.5f;
    rCamData.FLOAT_0xD4 = 0.5f;
    rCamData.FLOAT_0xD8 = 0.5f;
    rCamData.mViewportX = 0.0f;
    rCamData.mViewportY = 0.0f;
    rCamData.mViewportW = (float)viewportW;
    rCamData.mViewportH = (float)viewportH;
    rCamData.mViewportZStart = 0.0;
    rCamData.mViewportZEnd = 1.0f;
    rCamData.mScissorX = 0;
    rCamData.mScissorY = 0;
    rCamData.mScissorW = scissorWidth;
    rCamData.mScissorH = scissorHeight;
    rCamData.mScissorBoxOffsX = 0;
    rCamData.mScissorBoxOffsY = 0;
  }
}

void Camera::SetPosition(f32 x, f32 y, f32 z) {
  CameraData& rCamData = mCamData.ref();

  if (mCamData.IsValid()) {
    rCamData.mPos.x = x;
    rCamData.mPos.y = y;
    rCamData.mPos.z = z;

    rCamData.mFlags &= ~0x8;
  }
}

void Camera::SetPosition(const math::VEC3& rPos) {
  CameraData& rCamData = mCamData.ref();

  if (mCamData.IsValid()) {
    rCamData.mPos = rPos;
    rCamData.mFlags &= ~0x8;
  }
}

void Camera::SetPosture(const PostureInfo& rPosture) {
  CameraData& rCamData = mCamData.ref();

  if (mCamData.IsValid()) {
    switch (rPosture.INT_0x0) {
    case 0:
      if (rCamData.mFlags & 0x1) {
        bool b = (rPosture.VEC3_0x04 != rCamData.VEC3_0x80);
        if (!b) {
          b = (rPosture.VEC3_0x10 != rCamData.VEC3_0x8C);
          if (!b)
            return;
        }
      }
      rCamData.mFlags &= ~0x7;
      rCamData.mFlags |= 0x1;
      rCamData.VEC3_0x80 = rPosture.VEC3_0x04;
      rCamData.VEC3_0x8C = rPosture.VEC3_0x10;
      rCamData.mFlags &= ~0x8;
      break;

    case 1:
      if (rCamData.mFlags & 0x2) {
        bool b = (rPosture.VEC3_0x1C != rCamData.VEC3_0x98);
        if (!b)
          return;
      }
      rCamData.mFlags &= ~0x7;
      rCamData.mFlags |= 0x2;
      rCamData.VEC3_0x98 = rPosture.VEC3_0x1C;
      rCamData.mFlags &= ~0x8;
      break;

    case 2:
      if (rCamData.mFlags & 0x4) {
        bool b = (rPosture.VEC3_0x10 != rCamData.VEC3_0x8C);
        if (!b && rPosture.FLOAT_0x28 == rCamData.FLOAT_0xA4)
          return;
      }
      rCamData.mFlags &= ~0x7;
      rCamData.mFlags |= 0x4;
      rCamData.VEC3_0x8C = rPosture.VEC3_0x10;
      rCamData.FLOAT_0xA4 = rPosture.FLOAT_0x28;
      rCamData.mFlags &= ~0x8;
      break;

    default:
      break;
    }
  }
}

void Camera::SetCameraMtxDirectly(const math::MTX34& rMtx) {
  CameraData& rCamData = mCamData.ref();

  if (mCamData.IsValid()) {
    PSMTXCopy(rMtx.mtx, rCamData.mCamMtx.mtx);
    rCamData.mFlags |= 0x8;
  }
}

void Camera::SetPerspective(f32 fovY, f32 aspect, f32 n, f32 f) {
  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  rCamData.mIsOrtho = 0;
  rCamData.mFovY = fovY;
  rCamData.mAspectRatio = aspect;
  rCamData.mNearPlane = n;
  rCamData.mFarPlane = f;
  rCamData.mFlags &= ~0xf0;
  rCamData.mFlags |= 0x20;
}

void Camera::SetOrtho(f32 top, f32 bottom, f32 left, f32 right, f32 nearPlane,
                      f32 farPlane) {
  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  rCamData.mIsOrtho = 1;

  rCamData.mTop = top;
  rCamData.mBottom = bottom;
  rCamData.mLeft = left;
  rCamData.mRight = right;
  rCamData.mNearPlane = nearPlane;
  rCamData.mFarPlane = farPlane;

  rCamData.mFlags &= ~0xf0;
  rCamData.mFlags |= 0x40;
}

void Camera::SetScissor(u32 x, u32 y, u32 w, u32 h) {
  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  rCamData.mScissorX = x;
  rCamData.mScissorY = y;
  rCamData.mScissorW = w;
  rCamData.mScissorH = h;
}

void Camera::SetScissorBoxOffset(s32 x, s32 y) {
  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  rCamData.mScissorBoxOffsX = x;
  rCamData.mScissorBoxOffsY = y;
}

void Camera::SetProjectionMtxDirectly(const math::MTX44* rMtx) {
  if (!rMtx)
    return;

  CameraData& rCamData = mCamData.ref();

  if (!mCamData.IsValid())
    return;

  math::MTX44Copy(&rCamData.mProjMtx, (math::MTX44*)rMtx);
  rCamData.mFlags |= 0x80;
}

void Camera::SetViewport(f32 viewportX, f32 viewportY, f32 viewportW,
                         f32 viewportH) {
  CameraData& rCamData1 = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  rCamData1.mViewportX = viewportX;
  rCamData1.mViewportY = viewportY;
  rCamData1.mViewportW = viewportW;
  rCamData1.mViewportH = viewportH;

  CameraData& rCamData2 = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  rCamData2.mScissorX = viewportX;
  rCamData2.mScissorY = viewportY;
  rCamData2.mScissorW = viewportW;
  rCamData2.mScissorH = viewportH;
}

void Camera::SetViewportZRange(f32 viewportZStart, f32 viewportZEnd) {
  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  rCamData.mViewportZStart = viewportZStart;
  rCamData.mViewportZEnd = viewportZEnd;
}

void Camera::GetViewport(f32* viewportX, f32* viewportY, f32* viewportW,
                         f32* viewportH, f32* viewportZStart,
                         f32* viewportZEnd) const {
  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  if (viewportX)
    *viewportX = rCamData.mViewportX;

  if (viewportY)
    *viewportY = rCamData.mViewportY;

  if (viewportW)
    *viewportW = rCamData.mViewportW;

  if (viewportH)
    *viewportH = rCamData.mViewportH;

  if (viewportZStart)
    *viewportZStart = rCamData.mViewportZStart;

  if (!viewportZEnd)
    return;

  *viewportZEnd = rCamData.mViewportZEnd;
}

void Camera::GetCameraMtx(math::MTX34* dest) const {
  if (!dest)
    return;

  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;
  
  if ((rCamData.mFlags & 8) == 0) {
    UpdateCameraMtx();
  }

  PSMTXCopy(rCamData.mCamMtx.mtx, dest->mtx);
}

void Camera::GetProjectionMtx(math::MTX44* dest) const {
  if (!dest)
    return;

  CameraData& rCamData = mCamData.ref();
  if (!mCamData.IsValid())
    return;

  if((rCamData.mFlags & 0x80) == 0) {
    UpdateProjectionMtx();
    rCamData.mFlags |= 0x80;
  }

  math::MTX44Copy(dest, &rCamData.mProjMtx);
}
} // namespace g3d
} // namespace nw4r
