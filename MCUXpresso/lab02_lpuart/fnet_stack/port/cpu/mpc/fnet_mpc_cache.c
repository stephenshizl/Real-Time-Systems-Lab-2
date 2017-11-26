/**************************************************************************
*
* Copyright 2012-2016 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************/ /*!
*
* @brief MPC-specific cache invalidation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_MPC

#include "port/cpu/fnet_cpu.h"

/************************************************************************
* DESCRIPTION: Invalidate cache.
*************************************************************************/
#if FNET_CFG_CPU_CACHE
void fnet_cpu_cache_invalidate(void)
{
#if FNET_CFG_CPU_MPC5744P
    fnet_mpc5744p_dcache_disable();
#endif
}
#endif
#endif /*FNET_MPC*/

