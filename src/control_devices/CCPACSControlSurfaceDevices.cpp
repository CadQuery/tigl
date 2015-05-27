/*
 * Copyright (C) 2007-2013 German Aerospace Center (DLR/SC)
 *
 * Created: 2014-01-28 Mark Geiger <Mark.Geiger@dlr.de>
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

#include <iostream>
#include <sstream>
#include <exception>
#include <vector>

#include "CCPACSControlSurfaceDevices.h"
#include "CCPACSControlSurfaceDevice.h"
#include "CCPACSWingComponentSegment.h"

namespace tigl
{

CCPACSControlSurfaceDevices::CCPACSControlSurfaceDevices(CCPACSWingComponentSegment* cs)
    : _componentSegment(cs)
{
}

CCPACSControlSurfaceDevices::~CCPACSControlSurfaceDevices()
{
    Cleanup();
}

void CCPACSControlSurfaceDevices::Cleanup()
{
    // @todo: memory leak - use shared pointer or delete here
    for (std::size_t i = 0; i < controlSurfaceDevices.size(); i++) {
        controlSurfaceDevices.pop_back();
    }
}

// Read CPACS TrailingEdgeDevices elements
void CCPACSControlSurfaceDevices::ReadCPACS(
        TixiDocumentHandle tixiHandle,
        const std::string& controlSurfaceDevicesXPath, TiglControlSurfaceType type)
{
    ReturnCode tixiRet;
    int controlSurfaceDeviceCount;
    std::string tempString;
    char* elementPath;

    // Get ControlSurfaceDevive element count
    tempString = controlSurfaceDevicesXPath;
    elementPath = const_cast<char*>(tempString.c_str());

    if (type == TRAILING_EDGE_DEVICE) {
        tixiRet = tixiGetNamedChildrenCount(tixiHandle, elementPath,
                "trailingEdgeDevice", &controlSurfaceDeviceCount);
    }
    else if (type == LEADING_EDGE_DEVICE) {
        tixiRet = tixiGetNamedChildrenCount(tixiHandle, elementPath,
                "leadingEdgeDevice", &controlSurfaceDeviceCount);
    }
    else if (type == SPOILER) {
        tixiRet = tixiGetNamedChildrenCount(tixiHandle, elementPath,
                "spoiler", &controlSurfaceDeviceCount);
    }

    if (tixiRet != SUCCESS) {
        return;
    }

    // Loop over all controlSurfaces
    for (int i = 1; i <= controlSurfaceDeviceCount; i++) {
        CCPACSControlSurfaceDevice* controlSurfaceDevice =
                new CCPACSControlSurfaceDevice(_componentSegment);
        controlSurfaceDevices.push_back(controlSurfaceDevice);


        if (type == LEADING_EDGE_DEVICE) {
            tempString = controlSurfaceDevicesXPath + "/leadingEdgeDevice[";
        }
        else if (type == TRAILING_EDGE_DEVICE) {
            tempString = controlSurfaceDevicesXPath + "/trailingEdgeDevice[";
        }
        else if (type == SPOILER) {
            tempString = controlSurfaceDevicesXPath + "/spoiler[";
        }

        std::ostringstream xpath;
        xpath << tempString << i << "]";
        controlSurfaceDevice->ReadCPACS(tixiHandle, xpath.str(), type);
    }
}

CCPACSControlSurfaceDevice& CCPACSControlSurfaceDevices::getControlSurfaceDeviceByID(
        int id)
{
    return *controlSurfaceDevices[id-1];
}

int CCPACSControlSurfaceDevices::getControlSurfaceDeviceCount()
{
    return controlSurfaceDevices.size();
}

} // end namespace tigl
