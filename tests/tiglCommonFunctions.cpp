/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2015-05-08 Martin Siggel <Martin.Siggel@dlr.de>
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
#include "tiglcommonfunctions.h"
#include "test.h"

#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

TEST(TiglCommonFunctions, isPathRelative)
{
    ASSERT_TRUE(IsPathRelative("./test.txt"));

    ASSERT_TRUE(IsPathRelative("test.txt"));

    ASSERT_TRUE(IsPathRelative("../test.txt"));

    // check absolute paths
#ifdef WIN32
    ASSERT_FALSE(IsPathRelative("d:/data/test.txt"));
#else
    ASSERT_FALSE(IsPathRelative("/usr/bin/test.txt"));
#endif
}

TEST(TiglCommonFunctions, isFileReadable)
{
    ASSERT_TRUE(IsFileReadable("TestData/nacelle.stp"));
    ASSERT_FALSE(IsFileReadable("invalidfile.txt"));
}

TEST(TiglCommonFunctions, projectPointOnPlane)
{
    gp_Pnt p(0,0,1);
    gp_Pln pln(gp_Pnt(0,0,0), gp_Dir(0,0,1));

    gp_Pnt2d res;
    res = ProjectPointOnPlane(pln, p);
    ASSERT_NEAR(0, res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);

    p = gp_Pnt(2,0,3);
    res = ProjectPointOnPlane(pln, p);
    ASSERT_NEAR(2, res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);

    pln = gp_Pln(gp_Pnt(0,0,0), gp_Dir(0,1,0));
    res = ProjectPointOnPlane(pln, p);
    ASSERT_NEAR(3, res.X(), 1e-10);
    ASSERT_NEAR(2, res.Y(), 1e-10);

    pln = gp_Pln(gp_Pnt(0,0,0), gp_Dir(-1,1,0));
    p = gp_Pnt(1,0,0);
    res = ProjectPointOnPlane(pln, p);
    ASSERT_NEAR(sqrt(0.5), res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);

    pln = gp_Pln(gp_Pnt(1,0,0), gp_Dir(-1,1,0));
    p = gp_Pnt(1,0,0);
    res = ProjectPointOnPlane(pln, p);
    ASSERT_NEAR(0, res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);
}

TEST(TiglCommonFunctions, projectVecOnPlane)
{
    gp_Vec v(0,0,1);
    gp_Pln pln(gp_Pnt(0,0,0), gp_Dir(0,0,1));

    gp_Vec2d res;
    res = ProjectVecOnPlane(pln, v);
    ASSERT_NEAR(0, res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);

    v = gp_Vec(2,0,3);
    res = ProjectVecOnPlane(pln, v);
    ASSERT_NEAR(2, res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);

    pln = gp_Pln(gp_Pnt(0,0,0), gp_Dir(0,1,0));
    res = ProjectVecOnPlane(pln, v);
    ASSERT_NEAR(3, res.X(), 1e-10);
    ASSERT_NEAR(2, res.Y(), 1e-10);

    pln = gp_Pln(gp_Pnt(0,0,0), gp_Dir(-1,1,0));
    v = gp_Vec(1,0,0);
    res = ProjectVecOnPlane(pln, v);
    ASSERT_NEAR(sqrt(0.5), res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);

    pln = gp_Pln(gp_Pnt(1,0,0), gp_Dir(-1,1,0));
    v = gp_Vec(1,0,0);
    res = ProjectVecOnPlane(pln, v);
    ASSERT_NEAR(sqrt(0.5), res.X(), 1e-10);
    ASSERT_NEAR(0, res.Y(), 1e-10);
}

TEST(TiglCommonFunctions, IntersectLinePlane)
{
    gp_Pln plane(gp_Pnt(10., 2., 0.), gp_Dir(0., 1., 0));

    gp_Pnt result;
    ASSERT_EQ(BetweenPoints, IntersectLinePlane(gp_Pnt(0., 0., 0.), gp_Pnt(0., 4., 0.), plane, result));
    ASSERT_NEAR(0., result.Distance(gp_Pnt(0., 2., 0)), 1e-10);

    ASSERT_EQ(OutsideBefore, IntersectLinePlane(gp_Pnt(1., 3., 0.), gp_Pnt(1., 4., 0.), plane, result));
    ASSERT_NEAR(0., result.Distance(gp_Pnt(1., 2., 0)), 1e-10);

    ASSERT_EQ(OutsideAfter, IntersectLinePlane(gp_Pnt(1., 0., 0.), gp_Pnt(1., 1., 0.), plane, result));
    ASSERT_NEAR(0., result.Distance(gp_Pnt(1., 2., 0)), 1e-10);

    ASSERT_EQ(NoIntersection, IntersectLinePlane(gp_Pnt(1., 3., 0.), gp_Pnt(10., 3., 0.), plane, result));
}

TEST(TiglCommonFunctions, IsPointInsideShape)
{
    TopoDS_Shape box = BRepPrimAPI_MakeBox(1., 1., 1).Solid();

    EXPECT_TRUE(IsPointInsideShape(box, gp_Pnt(0.5, 0.5, 0.5)));
    EXPECT_TRUE(IsPointInsideShape(box, gp_Pnt(1.0, 0.5, 0.5)));

    EXPECT_FALSE(IsPointInsideShape(box, gp_Pnt(1.5, 0.5, 0.5)));

    TopoDS_Shape boxrev = box.Reversed();

    EXPECT_TRUE(IsPointInsideShape(boxrev, gp_Pnt(0.5, 0.5, 0.5)));
    EXPECT_TRUE(IsPointInsideShape(boxrev, gp_Pnt(1.0, 0.5, 0.5)));

    EXPECT_FALSE(IsPointInsideShape(boxrev, gp_Pnt(1.5, 0.5, 0.5)));

    // check tolerance of 1e-3
    EXPECT_TRUE(IsPointInsideShape(boxrev, gp_Pnt(1.0009, 0.5, 0.5)));
    EXPECT_FALSE(IsPointInsideShape(boxrev, gp_Pnt(1.0011, 0.5, 0.5)));

    // The function requires a solid, else it will throw
    TopoDS_Vertex v = BRepBuilderAPI_MakeVertex(gp_Pnt(10., 10., 10.));
    EXPECT_THROW(IsPointInsideShape(v, gp_Pnt(0., 0., 0.)), tigl::CTiglError);
}

TEST(TiglCommonFunctions, tiglCheckPointInside_api)
{
    TixiDocumentHandle tixiSimpleWingHandle = -1;
    TiglCPACSConfigurationHandle tiglSimpleWingHandle = -1;

    ReturnCode tixiRet = tixiOpenDocument("TestData/simpletest.cpacs.xml", &tixiSimpleWingHandle);
    ASSERT_EQ (SUCCESS, tixiRet);

    TiglReturnCode tiglRet = tiglOpenCPACSConfiguration(tixiSimpleWingHandle, "Cpacs2Test", &tiglSimpleWingHandle);
    ASSERT_EQ(TIGL_SUCCESS, tiglRet);

    TiglBoolean isInside = TIGL_FALSE;

    ASSERT_EQ(TIGL_SUCCESS, tiglCheckPointInside(tiglSimpleWingHandle, 0., 0., 0., "segmentD150_Fuselage_1Segment2ID", &isInside));
    EXPECT_EQ(TIGL_TRUE, isInside);

    ASSERT_EQ(TIGL_SUCCESS, tiglCheckPointInside(tiglSimpleWingHandle, 0., 0., 2., "segmentD150_Fuselage_1Segment2ID", &isInside));
    EXPECT_EQ(TIGL_FALSE, isInside);

    // test errors
    EXPECT_EQ(TIGL_UID_ERROR, tiglCheckPointInside(tiglSimpleWingHandle, 0., 0., 0., "wrongUID", &isInside));
    EXPECT_EQ(TIGL_NOT_FOUND, tiglCheckPointInside(-1, 0., 0., 0., "wrongUID", &isInside));
    EXPECT_EQ(TIGL_NULL_POINTER, tiglCheckPointInside(tiglSimpleWingHandle, 0., 0., 0., NULL, &isInside));
    EXPECT_EQ(TIGL_NULL_POINTER, tiglCheckPointInside(tiglSimpleWingHandle, 0., 0., 0., "wrongUID", NULL));


}