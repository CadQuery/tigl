// Copyright (c) 2016 RISC Software GmbH
//
// This file was generated by CPACSGen from CPACS XML Schema (c) German Aerospace Center (DLR/SC).
// Do not edit, all changes are lost when files are re-generated.
//
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "CPACSGuideCurve.h"
#include "CPACSGuideCurves.h"
#include "CTiglError.h"
#include "CTiglLogging.h"
#include "CTiglUIDManager.h"
#include "TixiHelper.h"

namespace tigl
{
    namespace generated
    {
        CPACSGuideCurves::CPACSGuideCurves(CTiglUIDManager* uidMgr) :
            m_uidMgr(uidMgr) {}
        
        CPACSGuideCurves::~CPACSGuideCurves() {}
        
        CTiglUIDManager& CPACSGuideCurves::GetUIDManager()
        {
            return *m_uidMgr;
        }
        
        const CTiglUIDManager& CPACSGuideCurves::GetUIDManager() const
        {
            return *m_uidMgr;
        }
        
        void CPACSGuideCurves::ReadCPACS(const TixiDocumentHandle& tixiHandle, const std::string& xpath)
        {
            // read element guideCurve
            if (tixi::TixiCheckElement(tixiHandle, xpath + "/guideCurve")) {
                tixi::TixiReadElements(tixiHandle, xpath + "/guideCurve", m_guideCurves, m_uidMgr);
            }
            
        }
        
        void CPACSGuideCurves::WriteCPACS(const TixiDocumentHandle& tixiHandle, const std::string& xpath) const
        {
            // write element guideCurve
            tixi::TixiSaveElements(tixiHandle, xpath + "/guideCurve", m_guideCurves);
            
        }
        
        const std::vector<unique_ptr<CPACSGuideCurve> >& CPACSGuideCurves::GetGuideCurves() const
        {
            return m_guideCurves;
        }
        
        std::vector<unique_ptr<CPACSGuideCurve> >& CPACSGuideCurves::GetGuideCurves()
        {
            return m_guideCurves;
        }
        
        CPACSGuideCurve& CPACSGuideCurves::AddGuideCurve()
        {
            m_guideCurves.push_back(make_unique<CPACSGuideCurve>(m_uidMgr));
            return *m_guideCurves.back();
        }
        
        void CPACSGuideCurves::RemoveGuideCurve(CPACSGuideCurve& ref)
        {
            for (std::size_t i = 0; i < m_guideCurves.size(); i++) {
                if (m_guideCurves[i].get() == &ref) {
                    m_guideCurves.erase(m_guideCurves.begin() + i);
                    return;
                }
            }
            throw CTiglError("Element not found");
        }
        
    }
}
