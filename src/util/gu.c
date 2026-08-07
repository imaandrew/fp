#include "gu.h"
#include <math.h>
#include <n64.h>

void guMtxIdent(Mtx *m) {
    *m = (Mtx)gdSPDefMtx(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

void guMtxIdentF(MtxF *mf) {
    *mf = (MtxF)guDefMtxF(1.F, 0.F, 0.F, 0.F, 0.F, 1.F, 0.F, 0.F, 0.F, 0.F, 1.F, 0.F, 0.F, 0.F, 0.F, 1.F);
}

void guPerspectiveF(MtxF *mf, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    f32 cot = cos(fovy / 2.F) / sin(fovy / 2.F);
    mf->xx = cot / aspect * scale;
    mf->xy = 0.F;
    mf->xz = 0.F;
    mf->xw = 0.F;
    mf->yx = 0.F;
    mf->yy = cot * scale;
    mf->yz = 0.F;
    mf->yw = 0.F;
    mf->zx = 0.F;
    mf->zy = 0.F;
    mf->zz = (near + far) / (near - far) * scale;
    mf->zw = -1.F * scale;
    mf->wx = 0.F;
    mf->wy = 0.F;
    mf->wz = 2.F * near * far / (near - far) * scale;
    mf->ww = 0.F;
}

void guMtxCatF(const MtxF *m, const MtxF *n, MtxF *r) {
    MtxF t;
    t.xx = m->xx * n->xx + m->xy * n->yx + m->xz * n->zx + m->xw * n->wx;
    t.xy = m->xx * n->xy + m->xy * n->yy + m->xz * n->zy + m->xw * n->wy;
    t.xz = m->xx * n->xz + m->xy * n->yz + m->xz * n->zz + m->xw * n->wz;
    t.xw = m->xx * n->xw + m->xy * n->yw + m->xz * n->zw + m->xw * n->ww;
    t.yx = m->yx * n->xx + m->yy * n->yx + m->yz * n->zx + m->yw * n->wx;
    t.yy = m->yx * n->xy + m->yy * n->yy + m->yz * n->zy + m->yw * n->wy;
    t.yz = m->yx * n->xz + m->yy * n->yz + m->yz * n->zz + m->yw * n->wz;
    t.yw = m->yx * n->xw + m->yy * n->yw + m->yz * n->zw + m->yw * n->ww;
    t.zx = m->zx * n->xx + m->zy * n->yx + m->zz * n->zx + m->zw * n->wx;
    t.zy = m->zx * n->xy + m->zy * n->yy + m->zz * n->zy + m->zw * n->wy;
    t.zz = m->zx * n->xz + m->zy * n->yz + m->zz * n->zz + m->zw * n->wz;
    t.zw = m->zx * n->xw + m->zy * n->yw + m->zz * n->zw + m->zw * n->ww;
    t.wx = m->wx * n->xx + m->wy * n->yx + m->wz * n->zx + m->ww * n->wx;
    t.wy = m->wx * n->xy + m->wy * n->yy + m->wz * n->zy + m->ww * n->wy;
    t.wz = m->wx * n->xz + m->wy * n->yz + m->wz * n->zz + m->ww * n->wz;
    t.ww = m->wx * n->xw + m->wy * n->yw + m->wz * n->zw + m->ww * n->ww;
    *r = t;
}

void guRotateF(MtxF *mf, f32 a, f32 x, f32 y, f32 z) {
    f32 s = sin(a);
    f32 c = cos(a);
    mf->xx = x * x + c * (1.F - x * x);
    mf->xy = x * y * (1.F - c) + z * s;
    mf->xz = x * z * (1.F - c) - y * s;
    mf->xw = 0.F;
    mf->yx = y * x * (1.F - c) - z * s;
    mf->yy = y * y + c * (1.F - y * y);
    mf->yz = y * z * (1.F - c) + x * s;
    mf->yw = 0.F;
    mf->zx = z * x * (1.F - c) + y * s;
    mf->zy = z * y * (1.F - c) - x * s;
    mf->zz = z * z + c * (1.F - z * z);
    mf->zw = 0.F;
    mf->wx = 0.F;
    mf->wy = 0.F;
    mf->wz = 0.F;
    mf->ww = 1.F;
}

void guRotateRPYF(MtxF *mf, f32 r, f32 p, f32 h) {
    f32 sr = sin(r);
    f32 cr = cos(r);
    f32 sp = sin(p);
    f32 cp = cos(p);
    f32 sh = sin(h);
    f32 ch = cos(h);
    mf->xx = cp * ch;
    mf->xy = cp * sh;
    mf->xz = -sp;
    mf->xw = 0.F;
    mf->yx = sr * sp * ch - cr * sh;
    mf->yy = sr * sp * sh + cr * ch;
    mf->yz = sr * cp;
    mf->yw = 0.F;
    mf->zx = cr * sp * ch + sr * sh;
    mf->zy = cr * sp * sh - sp * sh;
    mf->zz = cr * cp;
    mf->zw = 0.F;
    mf->wx = 0.F;
    mf->wy = 0.F;
    mf->wz = 0.F;
    mf->ww = 1.F;
}

void guScaleF(MtxF *mf, f32 x, f32 y, f32 z) {
    *mf = (MtxF)guDefMtxF(x, 0.F, 0.F, 0.F, 0.F, y, 0.F, 0.F, 0.F, 0.F, z, 0.F, 0.F, 0.F, 0.F, 1.F);
}

void guTranslateF(MtxF *mf, f32 x, f32 y, f32 z) {
    *mf = (MtxF)guDefMtxF(1.F, 0.F, 0.F, 0.F, 0.F, 1.F, 0.F, 0.F, 0.F, 0.F, 1.F, 0.F, x, y, z, 1.F);
}

void guMtxF2L(const MtxF *mf, Mtx *m) {
    for (s32 i = 0; i < 16; ++i) {
        qs1616_t n = qs1616(mf->f[i]);
        m->i[i] = (n >> 16) & 0x0000FFFF;
        m->f[i] = (n >> 0) & 0x0000FFFF;
    }
}
