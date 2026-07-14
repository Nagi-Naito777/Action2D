#include "Collision.h"

bool IsHitAABB(const Rect& rectA, const Rect& rectB) {
    if (rectA.Right() <= rectB.Left())   return false;
    if (rectA.Left() >= rectB.Right())   return false;
    if (rectA.Bottom() <= rectB.Top())   return false;
    if (rectA.Top() >= rectB.Bottom())   return false;
    return true;
}