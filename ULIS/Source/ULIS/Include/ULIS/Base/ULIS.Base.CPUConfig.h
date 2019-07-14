/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS.Base.CPUConfig.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#include "ULIS/Base/ULIS.Base.BaseTypes.h"
#include "ULIS/Base/FeatureDetector/cpu_x86.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FCPUConfig
struct FCPUConfig
{
    FCPUConfig()
    {
        info.detect_host();
    }

    ::FeatureDetector::cpu_x86 info;
};

} // namespace ULIS
