/*
* Copyright (C) 2018 German Aerospace Center (DLR/SC)
*
* Created: 2018-11-16 Jan Kleinert <jan.kleinert@dlr.de>
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

#include "CCPACSNacelleSection.h"
#include "CCPACSNacelleProfile.h"
#include "CTiglUIDManager.h"
#include "tiglcommonfunctions.h"

namespace{
tigl::CTiglTransformation GetTransformationMatrix(const tigl::CCPACSTransformationPolar& trans);
}

namespace tigl {

TIGL_EXPORT CCPACSNacelleSection::CCPACSNacelleSection(CTiglUIDManager* uidMgr)
   : generated::CPACSNacelleSection(uidMgr)
{};

TIGL_EXPORT TopoDS_Wire CCPACSNacelleSection::GetTransformedWire()
{
    // get untransformed profile wire
    const CCPACSNacelleProfile& profile = m_uidMgr->ResolveObject<CCPACSNacelleProfile>(m_profileUID);

    // apply polar transformation
    TopoDS_Shape transformedShape(profile.GetWire());
    CTiglTransformation trafo = GetTransformationMatrix(m_transformation);
    transformedShape = trafo.Transform(transformedShape);
    return TopoDS::Wire(transformedShape);
}

} //namepsace tigl

namespace {

// TODO: 2D Polar coordinates cannot be used to place things in 3D space. CCPACSTransformationPolar
// should not be used anywhere but here.
tigl::CTiglTransformation GetTransformationMatrix(const tigl::CCPACSTransformationPolar& trans)
{
    tigl::CTiglTransformation out;

    // get r and phi from translation
    double radius = 0;
    double phi    = 0;
    if ( trans.GetTranslation() ) {
        if (trans.GetTranslation()->GetR()) { radius = trans.GetTranslation()->GetR().get();   }
        if (trans.GetTranslation()->GetR()) { phi    = trans.GetTranslation()->GetPhi().get(); }
    }

    // rotate from XY-plane to XZ-plane
    out.AddRotationX(90.);

    // apply scaling
    if ( trans.GetScaling() ) {
        tigl::CTiglPoint scale = trans.GetScaling()->AsPoint();
        out.AddScaling(scale.x, scale.y, scale.z);
    }

    // apply rotation
    if ( trans.GetRotation() ) {
        tigl::CTiglPoint rotation = trans.GetRotation()->AsPoint();
        out.AddRotationX(rotation.x);
        out.AddRotationY(rotation.y);
        out.AddRotationZ(rotation.z);
    }

    // apply translation (and rotate the profile accordingly)
    out.AddRotationX(phi);
    out.AddTranslation(0,-radius*sin(Radians(phi)),radius*cos(Radians(phi)));

    return out;
}

}