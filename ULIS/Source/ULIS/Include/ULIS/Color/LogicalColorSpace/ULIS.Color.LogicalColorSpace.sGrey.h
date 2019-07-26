/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS.Color.LogicalColorSpace.sGrey.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#pragma once

#include "ULIS/Color/ULIS.Color.LogicalColorSpace.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FColorSpacesGrey
class FColorSpacesGrey : public ILogicalColorSpace
{
public:
    virtual e_cm AssociatedModel() const override { return e_cm::kG; }
};

} // namespace ULIS
