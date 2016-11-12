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
	else if (plug == aNormalOffset || plug == aTranslateOffset || plug == aRotationOffset || plug == aScaleOffset)
	{
		attributeDirty_[kOffsets] = true;
	}
	else if (plug == aNormalRandom || plug == aTranslateRandom || plug == aRotationRandom || plug == aScaleRandom)
	{
		attributeDirty_[kRandoms] = true;
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

	MObject outMesh = data.outputValue(DLInstancer::aOutMesh, &status).asMesh();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	bool recreateOutMesh = false;
	bool recreateMatricies = false;
	MDataHandle output = data.outputValue(aOutMesh);



	if (attributeDirty_[kInstanceMesh] == true)
	{
		MObject instanceMesh = data.inputValue(DLInstancer::aInstanceMesh, &status).asMesh();
		CHECK_MSTATUS_AND_RETURN_IT(status);

	
		status = dlGetMeshData(instanceMesh, inputInstanceMeshData_);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		recreateOutMesh = true;
	}

	if (attributeDirty_[kReferenceMesh] == true)
	{
		MObject referenceMesh = data.inputValue(DLInstancer::aReferenceMesh, &status).asMesh();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MFnMesh fnRefMesh(referenceMesh);
		MPointArray points;
		MFloatVectorArray normals;

		status = fnRefMesh.getPoints(points, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = fnRefMesh.getVertexNormals(true, normals, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		if (points.length() != numInstances_)
		{
			numInstances_ = points.length();
			recreateOutMesh = true;
		}

		transformData_.referencePoints = points;
		transformData_.referenceNormals = normals;

		//calculate normal rotations and store in transform data

		recreateMatricies = true;
	}

	if (attributeDirty_[kOffsets] == true)
	{
		float normalOffset = data.inputValue(DLInstancer::aNormalOffset, &status).asFloat();
		float3& translateOffset = data.inputValue(DLInstancer::aTranslateOffset, &status).asFloat3();
		float3& rotationOffset = data.inputValue(DLInstancer::aRotationOffset, &status).asFloat3();
		float3& scaleOffset = data.inputValue(DLInstancer::aScaleOffset, &status).asFloat3();

		transformData_.normalOffset = normalOffset;
		dlCopyFloat3(transformData_.translateOffset, translateOffset);
		dlCopyFloat3(transformData_.rotationOffset, rotationOffset);
		dlCopyFloat3(transformData_.scaleOffset, scaleOffset);

		recreateMatricies = true;
	}

	if (attributeDirty_[kRandoms] == true)
	{
		/*float maxNormalRandom = data.inputValue(DLInstancer::aNormalRandom, &status).asFloat();
		float3& maxTranslateRandom = data.inputValue(DLInstancer::aTranslateRandom, &status).asFloat3();
		float3& maxRotationRandom = data.inputValue(DLInstancer::aRotationRandom, &status).asFloat3();
		float3& maxScaleRandom = data.inputValue(DLInstancer::aScaleRandom, &status).asFloat3();

		float normalRandom;
		float3 translateRandom;
		float3 rotationRandom;
		float3 scaleRandom;


		transformData_.normalRandom = normalRandom;
		dlCopyFloat3(transformData_.translateOffset, translateRandom);
		dlCopyFloat3(transformData_.rotationOffset, rotationRandom);
		dlCopyFloat3(transformData_.scaleOffset, scaleRandom);

		recreateMatricies = true;*/
	}


	if (recreateOutMesh == true)
	{
		status = dlCreateOutputMeshData(inputInstanceMeshData_, numInstances_, outputInstanceMeshData_);
		outMesh = dlCreateMesh(outputInstanceMeshData_);

	}

	if (recreateMatricies == true)
	{
		//copy current matricies to old set
		//create new Matricies
	}
	
	
	//Deform Out Mesh

	output.set(outMesh);
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

	MString map = "map1";
	status = fnMesh.getUVs(meshData.uArray, meshData.vArray, &map);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (; !itPoly.isDone(); itPoly.next())
	{
		int numVerts = itPoly.polygonVertexCount(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = meshData.polyCounts.append(numVerts);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MIntArray vertIDs;
		status = itPoly.getVertices(vertIDs);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		for (unsigned int i = 0; i < vertIDs.length(); ++i)
		{
			CHECK_MSTATUS_AND_RETURN_IT(status);
			status = meshData.polyConnects.append(vertIDs[i]);
		}

		MFloatArray tempUV;
		status = itPoly.getUVs(tempUV, tempUV);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = meshData.uvCounts.append( tempUV.length() );
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	for (; !itFaceVert.isDone(); itFaceVert.next())
	{
		int uvID;
		MString map = "map1";
		status = itFaceVert.getUVIndex(uvID, &map);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = meshData.uvIDs.append(uvID);
		CHECK_MSTATUS_AND_RETURN_IT(status);
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

MObject DLInstancer::dlCreateMesh(const DLMeshData& meshData)
{
	MStatus status;
	MFnMeshData  dataFn;
	MObject dataWrapper = dataFn.create();
	MFnMesh generator;

	MObject outMesh = generator.create(meshData.numPoints, meshData.numPolys, 
		meshData.pointArray, meshData.polyCounts, meshData.polyConnects, 
		meshData.uArray, meshData.vArray, dataWrapper, &status);

	
	if (status != MS::kSuccess)
	{
		MGlobal::displayError(status.errorString());
		MGlobal::displayError("Problem Creating Mesh. Returning Null Mesh");
		MFnMeshData fnNullMesh;
		return fnNullMesh.create();
	}

	MFnMesh fnOutMesh(dataWrapper);

	MString map = "map1";
	fnOutMesh.assignUVs(meshData.uvCounts, meshData.uvIDs, &map);
	if (status != MS::kSuccess)
	{
		MGlobal::displayError(status.errorString());
		MGlobal::displayError("Problem Assigning UVs");
	}


	return dataWrapper;
}

MStatus DLInstancer::dlCalculateVectorAngles(float3 base, float3 direction, float3 & angles)
{
	MStatus status;


	return MS::kSuccess;
}

MMatrixArray DLInstancer::dlGenerateMatricies(const DLTransformData& transformData)
{
	MStatus status;

	MMatrixArray outMatrixArray;

	for (unsigned int i = 0; i < numInstances_; ++i)
	{
		MTransformationMatrix transformMatrix;
		MVector translation(transformData.referencePoints[i]);
		transformMatrix.setTranslation(translation, MSpace::kWorld);
		outMatrixArray.append(transformMatrix.asMatrix());
	}
	return outMatrixArray;
}

MStatus DLInstancer::dlDeformMesh(MObject& mesh, MMatrixArray& matricies, bool usePeviousMatrix)
{
	MStatus status;
	MItGeometry itGeo(mesh);
	MFnMesh fnMesh(mesh);
	unsigned int numBaseMeshPoints = fnMesh.numVertices() / numInstances_;
	MPoint position;
	unsigned int matrixIndex = 0;
	unsigned int curIndex = 0;

	for (; !itGeo.isDone(); itGeo.next(), ++curIndex);
	{
		position = itGeo.position(MSpace::kWorld);
		if (usePeviousMatrix)
		{
			position *= previousTransformMatricies_[matrixIndex].inverse();
		}
		position *= ouputTransformMatricies_[matrixIndex];
		itGeo.setPosition(position, MSpace::kWorld);

		if (curIndex % numBaseMeshPoints == 0 && curIndex != 0)
		{
			matrixIndex += 1;
		}
	}

	return MS::kSuccess;
}


