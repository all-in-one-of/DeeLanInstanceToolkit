#include "DLInstancer.h"

//Type and ID
MTypeId DLInstancer::id(0x00000000);
const MString DLInstancer::nodeName = "dlInstancer";

//Input Attributes
MObject DLInstancer::aInstanceMesh;
MObject DLInstancer::aReferenceMesh;
MObject DLInstancer::aNormalOffset;
MObject DLInstancer::aTranslateOffset;
MObject DLInstancer::aRotationOffset;
MObject DLInstancer::aScaleOffset;
MObject DLInstancer::aNormalRandom;
MObject DLInstancer::aTranslateRandom;
MObject DLInstancer::aRotationRandom;
MObject DLInstancer::aScaleRandom;
//Output Attributes
MObject DLInstancer::aOutMesh;
MObject DLInstancer::aInstanceGroup;
MObject DLInstancer::aInstanceGroupMesh;
MObject DLInstancer::aInstanceGroupMatricies;

DLInstancer::DLInstancer() 
{
}

DLInstancer::~DLInstancer()
{
}

void * DLInstancer::creator()
{
	return new DLInstancer;
}

MStatus DLInstancer::initialize()
{
	MStatus status;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnCompoundAttribute cAttr;
	MFnMatrixAttribute mAttr;

	//Input Attributes
	aInstanceMesh = tAttr.create("instanceMesh", "iMesh", MFnData::kMesh);
	tAttr.setKeyable(true);
	tAttr.setReadable(false);
	addAttribute(aInstanceMesh);

	aReferenceMesh = tAttr.create("referenceMesh", "rMesh", MFnData::kMesh);
	tAttr.setKeyable(true);
	tAttr.setReadable(false);
	addAttribute(aReferenceMesh);

	aNormalOffset = nAttr.create("normalOffset", "nOff", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aNormalOffset);

	aTranslateOffset = nAttr.create("translateOffset", "tOff", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aTranslateOffset);

	aRotationOffset = nAttr.create("rotationOffset", "rOff", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aRotationOffset);

	aScaleOffset = nAttr.create("scaleOffset", "sOff", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aScaleOffset);

	aNormalRandom = nAttr.create("normalRandom", "nRand", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aNormalRandom);

	aTranslateRandom = nAttr.create("translateRandom", "tRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aTranslateRandom);

	aRotationRandom = nAttr.create("rotationRandom", "rRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aRotationRandom);

	aScaleRandom = nAttr.create("scaleRandom", "sRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aScaleRandom);

	//Output Attributes
	aOutMesh = tAttr.create("outMesh", "oMesh", MFnData::kMesh);
	tAttr.setStorable(false);
	tAttr.setWritable(false);
	addAttribute(aOutMesh);


	aInstanceGroup = cAttr.create("instanceGroup", "instGrp");
	cAttr.setStorable(false);
	cAttr.setWritable(false);

	aInstanceGroupMesh = tAttr.create("instanceGroupMesh", "instGrpMesh", MFnData::kMesh);
	tAttr.setStorable(false);
	tAttr.setWritable(false);

	aInstanceGroupMatricies = mAttr.create("instanceGroupMatricies", "instGrpMatricies");
	mAttr.setArray(true);
	mAttr.setStorable(false);
	mAttr.setWritable(false);

	cAttr.addChild(aInstanceGroupMesh);
	cAttr.addChild(aInstanceGroupMatricies);
	addAttribute(aInstanceGroup);

	//Attribute Relationships
	attributeAffects(aInstanceMesh, aOutMesh);
	attributeAffects(aReferenceMesh, aOutMesh);
	attributeAffects(aNormalOffset, aOutMesh);
	attributeAffects(aTranslateOffset, aOutMesh);
	attributeAffects(aRotationOffset, aOutMesh);
	attributeAffects(aScaleOffset, aOutMesh);
	attributeAffects(aNormalRandom, aOutMesh);
	attributeAffects(aTranslateRandom, aOutMesh);
	attributeAffects(aRotationRandom, aOutMesh);
	attributeAffects(aScaleRandom, aOutMesh);

	/*
	attributeAffects(aInstanceMesh, aInstanceGroupMesh);
	attributeAffects(aReferenceMesh, aInstanceGroupMatricies);
	attributeAffects(aNormalOffset, aInstanceGroupMatricies);
	attributeAffects(aTranslateOffset, aInstanceGroupMatricies);
	attributeAffects(aRotationOffset, aInstanceGroupMatricies);
	attributeAffects(aScaleOffset, aInstanceGroupMatricies);
	attributeAffects(aNormalRandom, aInstanceGroupMatricies);
	attributeAffects(aTranslateRandom, aInstanceGroupMatricies);
	attributeAffects(aRotationRandom, aInstanceGroupMatricies);
	attributeAffects(aScaleRandom, aInstanceGroupMatricies);
	*/

	


	return MS::kSuccess;
}

MStatus DLInstancer::setDependentsDirty(const MPlug &plug, MPlugArray &plugArray)
{
	MStatus status;

	if (plug == aInstanceMesh)
	{
		attributeDirty_[kInstanceMesh] = true;
	}
	else if (plug == aReferenceMesh)
	{
		attributeDirty_[kReferenceMesh] = true;
	}
	else if (plug == aNormalOffset)
	{
		attributeDirty_[kNormalOffset] = true;
	}
	else if (plug == aTranslateOffset)
	{
		attributeDirty_[kTranslateOffset] = true;
	}
	else if (plug == aRotationOffset)
	{
		attributeDirty_[kRotationOffset] = true;
	}
	else if (plug == aScaleOffset)
	{
		attributeDirty_[kScaleOffset] = true;
	}
	else if (plug == aNormalRandom)
	{
		attributeDirty_[kNormalRandom] = true;
	}
	else if (plug == aTranslateRandom)
	{
		attributeDirty_[kTranslateRandom] = true;
	}
	else if (plug == aRotationRandom)
	{
		attributeDirty_[kRotationRandom] = true;
	}
	else if (plug == aScaleRandom)
	{
		attributeDirty_[kScaleRandom] = true;
	}

	return MS::kSuccess;
}

MStatus DLInstancer::compute(const MPlug &plug, MDataBlock &data)
{
	MGlobal::displayInfo("COMPUTE CALLED!!!");
	MStatus status;

	if (plug != aOutMesh)
	{
		return MS::kUnknownParameter;
	}

	MPlug iterPlug = plug;

	MItDependencyGraph itDG(iterPlug);
	itDG.next();

	MObject parentNode = itDG.thisPlug().node();

	MGlobal::displayInfo(itDG.thisPlug().name());



	
	MObject instanceMesh = data.inputValue(DLInstancer::aInstanceMesh, &status).asMesh();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = dlGetMeshData(instanceMesh, inputInstanceMeshData_);

	MFnMeshData  dataFn;
	MObject dataWrapper = dataFn.create();
	MFnMesh generator;
	MObject outMesh = generator.create(inputInstanceMeshData_.numPoints, inputInstanceMeshData_.numPolys,
		inputInstanceMeshData_.pointArray, inputInstanceMeshData_.polyCounts, inputInstanceMeshData_.polyConnects,
		inputInstanceMeshData_.uArray, inputInstanceMeshData_.vArray, dataWrapper, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = generator.setNormals(inputInstanceMeshData_.normals);
	CHECK_MSTATUS_AND_RETURN_IT(status);



	MDataHandle output = data.outputValue(aOutMesh);
	output.set(dataWrapper);
	data.setClean(plug);
	
	


	return MS::kSuccess;
}

MStatus DLInstancer::dlGetMeshData(const MObject& mesh, DLMeshData& meshData)
{
	MStatus status;

	meshData.pointArray.clear();
	meshData.polyCounts.clear();
	meshData.polyConnects.clear();
	meshData.uArray.clear();
	meshData.vArray.clear();
	meshData.uvIDs.clear();
	meshData.uvCounts.clear();


	MFnMesh fnMesh(mesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MItMeshPolygon itPoly(mesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MItMeshFaceVertex itFaceVert(mesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	meshData.numPoints = fnMesh.numVertices();
	meshData.numPolys = fnMesh.numPolygons();
	status = fnMesh.getPoints(meshData.pointArray, MSpace::kWorld);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	//SORT OUT NORMALS
	status = fnMesh.getNormals(meshData.normals, MSpace::kWorld);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnMesh.getUVs(meshData.uArray, meshData.vArray);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (; !itPoly.isDone(); itPoly.next())
	{
		int numVerts = itPoly.polygonVertexCount(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		meshData.polyCounts.append(numVerts);

		MIntArray vertIDs;
		status = itPoly.getVertices(vertIDs);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		for (int i = 0; i < vertIDs.length(); ++i)
		{
			meshData.polyConnects.append(vertIDs[i]);
		}

		MFloatArray tempUV;
		status = itPoly.getUVs(tempUV, tempUV);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		meshData.uvCounts.append( tempUV.length() );

	}

	for (; !itFaceVert.isDone(); itFaceVert.next())
	{
		int uvID;
		status = itFaceVert.getUVIndex(uvID);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		meshData.uvIDs.append(uvID);
	}


	return MS::kSuccess;
}

MStatus DLInstancer::dlCreateOutputMeshData(const DLMeshData & inMeshData, unsigned int numCopies, DLMeshData & outMeshData, bool clearData)
{
	MStatus status;

	if (clearData)
	{
		outMeshData.pointArray.clear();
		outMeshData.polyCounts.clear();
		outMeshData.polyConnects.clear();
		outMeshData.uArray.clear();
		outMeshData.vArray.clear();
		outMeshData.uvIDs.clear();
		outMeshData.uvCounts.clear();
	}

	outMeshData.numPoints = inMeshData.numPoints * numCopies;
	outMeshData.numPolys = inMeshData.numPolys * numCopies;

	for (unsigned int i = 0; i < numCopies; ++i)
	{
		outMeshData.appendArrayData(inMeshData);
	}

	return MS::kSuccess;
}

MObject DLInstancer::dlCreateMesh(const DLMeshData & meshData)
{

	return MObject();
}


