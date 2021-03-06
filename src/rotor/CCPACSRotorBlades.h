/*
* Copyright (C) 2007-2014 German Aerospace Center (DLR/SC)
*
* Created: 2014-04-11 Philipp Kunze <Philipp.Kunze@dlr.de>
* Changed: $Id$
*
* Version: $Revision$
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
* @file
* @brief  Implementation of rotor blades handling routines.
*/

#ifndef CCPACSROTORBLADES_H
#define CCPACSROTORBLADES_H

#include "generated/CPACSRotorBlades.h"

namespace tigl
{

class CCPACSConfiguration;
class CCPACSWing;

class CCPACSRotorBlades : public generated::CPACSRotorBlades
{
public:
    // Constructor
    TIGL_EXPORT CCPACSRotorBlades(CCPACSRotorcraftModel* parent, CTiglUIDManager* uidMgr);

    // Invalidates internal state
    TIGL_EXPORT void Invalidate();

    // Adds a rotor blade to the  rotor blade container
    TIGL_EXPORT void AddRotorBlade(CCPACSWing* rotorBlade);
    using generated::CPACSRotorBlades::AddRotorBlade;

    // Returns the total count of rotor blades of a rotor blade attachment
    TIGL_EXPORT int GetRotorBladeCount() const;

    // Returns the rotor blade for a given index.
    TIGL_EXPORT CCPACSWing& GetRotorBlade(int index) const;

    // Returns the rotor blade for a given index.
    TIGL_EXPORT CCPACSWing& GetRotorBlade(const std::string& uid) const;

    TIGL_EXPORT int GetRotorBladeIndex(const std::string& uid) const;

    // Returns the parent configuration
    TIGL_EXPORT CCPACSConfiguration& GetConfiguration() const;
};

} // end namespace tigl

#endif // CCPACSROTORBLADES_H
