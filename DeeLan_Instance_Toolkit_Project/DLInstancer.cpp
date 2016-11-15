#include "DLInstancer.h"

//Type and ID
MTypeId DLInstancer::id(0x00000023);
const MString DLInstancer::nodeName = "dlInstancer";

//Input Attributes
MObject DLInstancer::aInstanceMesh;
MObject DLInstancer::aReferenceMesh;
MObject DLInstancer::aNormalOffset;
MObject DLInstancer::aTranslateOffset;
MObject DLInstancer::aRotationOffset;
MObject DLInstancer::aUniformScaleOffset;
MObject DLInstancer::aScaleOffset;
MObject DLInstancer::aNormalRandom;
MObject DLInstancer::aTranslateRandom;
MObject DLInstancer::aRotationRandom;
MObject DLInstancer::aUniformScaleRandom;
MObject DLInstancer::aScaleRandom;
MObject DLInstancer::aNodeSeed;
MObject DLInstancer::aGeneratedMesh;

//Output Attributes
MObject DLInstancer::aOutMesh;
MObject DLInstancer::aInstanceGroup;
MObject DLInstancer::aInstanceGroupMesh;
MObject DLInstancer::aInstanceGroupMatricies;

DLInstancer::DLInstancer()
{
	transformData_.scaleOffset = { 1.0, 1.0, 1.0 };
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

	aUniformScaleOffset = nAttr.create("uniformScaleOffset", "usOff", MFnNumericData::kFloat, 1.0f);
	nAttr.setKeyable(true);
	addAttribute(aUniformScaleOffset);

	aScaleOffset = nAttr.create("scaleOffset", "sOff", MFnNumericData::k3Float, (1.0f, 1.0f, 1.0f));
	nAttr.setKeyable(true);
	addAttribute(aScaleOffset);

	aNormalRandom = nAttr.create("normalRandom", "nRand", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	addAttribute(aNormalRandom);

	aTranslateRandom = nAttr.create("translateRandom", "tRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f, 0.0f, 0.0f);
	addAttribute(aTranslateRandom);

	aRotationRandom = nAttr.create("rotationRandom", "rRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f, 0.0f, 0.0f);
	addAttribute(aRotationRandom);

	aUniformScaleRandom = nAttr.create("uniformScaleRandom", "usRand", MFnNumericData::kFloat, 1.0f);
	nAttr.setKeyable(true);
	addAttribute(aUniformScaleRandom);

	aScaleRandom = nAttr.create("scaleRandom", "sRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f, 0.0f, 0.0f);
	addAttribute(aScaleRandom);

	aNodeSeed = nAttr.create("nodeSeed", "nSeed", MFnNumericData::kInt);
	nAttr.setKeyable(true);
	addAttribute(aNodeSeed);

	aGeneratedMesh = tAttr.create("generatedMesh", "gMesh", MFnData::kMesh);
	tAttr.setConnectable(false);
	tAttr.setHidden(true);
	addAttribute(aGeneratedMesh);

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
	attributeAffects(aUniformScaleOffset, aOutMesh);
	attributeAffects(aScaleOffset, aOutMesh);
	attributeAffects(aNormalRandom, aOutMesh);
	attributeAffects(aTranslateRandom, aOutMesh);
	attributeAffects(aRotationRandom, aOutMesh);
	attributeAffects(aUniformScaleRandom, aOutMesh);
	attributeAffects(aScaleRandom, aOutMesh);
	attributeAffects(aNodeSeed, aOutMesh);


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

	MGlobal::displayInfo("SET DEPENDENTS DIRTY CALLED!!!");

	if (plug == aInstanceMesh)
	{
		attributeDirty_[kInstanceMesh] = true;
	}
	else if (plug == aReferenceMesh)
	{
		attributeDirty_[kReferenceMesh] = true;
	}
	else if (plug == aNormalOffset || plug.parent() == aTranslateOffset || 
			 plug.parent() == aRotationOffset || plug.parent() == aScaleOffset ||
			 plug == aUniformScaleOffset)
	{
		attributeDirty_[kOffsets] = true;
	}
	else if (plug == aNormalRandom || plug.parent() == aTranslateRandom || 
			 plug.parent() == aRotationRandom || plug.parent() == aScaleRandom ||
			 plug == aUniformScaleRandom)
	{
		attributeDirty_[kRandoms] = true;
	}

	setDependentsDirtyCalled_ = true;
	return MS::kSuccess;
}

MStatus DLInstancer::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus status;

	if (plug != aOutMesh)
	{
		return MS::kUnknownParameter;
	}

	MGlobal::displayInfo("COMPUTE CALLED!!!");

	//Supporting Timeline Playback
	MAnimControl anim;
	MTime curTime = anim.currentTime();
	if (curTime != prevTime_)
	{
		prevTime_ = curTime;
		MGlobal::displayInfo("Time Is Different");
		if (setDependentsDirtyCalled_ == false)
		{
			MGlobal::displayInfo("setDepDirty not Called");
			dlManualSetDependentsDirty(data);
		}
	}
	setDependentsDirtyCalled_ = false;

	MDataHandle hOutput = data.outputValue(aOutMesh);
	MDataHandle hGeneratedMesh = data.outputValue(aGeneratedMesh);
	bool recreateOutMesh = false;
	bool recreateMatricies = false;




	if (attributeDirty_[kInstanceMesh] == true)
	{
		MGlobal::displayInfo("Instance Mesh");
		MObject instanceMesh = data.inputValue(DLInstancer::aInstanceMesh, &status).asMesh();
		CHECK_MSTATUS_AND_RETURN_IT(status);


		status = dlGetMeshData(instanceMesh, inputInstanceMeshData_);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		attributeDirty_[kInstanceMesh] = false;
		recreateOutMesh = true;
	}

	if (attributeDirty_[kReferenceMesh] == true)
	{
		MGlobal::displayInfo("Reference Mesh");
		MObject referenceMesh = data.inputValue(DLInstancer::aReferenceMesh, &status).asMesh();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MFnMesh fnRefMesh(referenceMesh);
		MPointArray points;
		MFloatVectorArray normals;

		status = fnRefMesh.getPoints(points, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = fnRefMesh.getVertexNormals(false, normals, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		if (points.length() != numInstances_)
		{
			numInstances_ = points.length();
			recreateOutMesh = true;
		}

		transformData_.referencePoints = points;
		transformData_.referenceNormals = normals;

		//calculate normal rotations and store in transform data
		attributeDirty_[kReferenceMesh] = false;
		recreateMatricies = true;
	}

	if (attributeDirty_[kOffsets] == true)
	{
		MGlobal::displayInfo("Offsets");

		float normalOffset = data.inputValue(DLInstancer::aNormalOffset, &status).asFloat();
		float3& translateOffset = data.inputValue(DLInstancer::aTranslateOffset, &status).asFloat3();
		float3& rotationOffset = data.inputValue(DLInstancer::aRotationOffset, &status).asFloat3();
		float uniformScaleOffset = data.inputValue(DLInstancer::aUniformScaleOffset, &status).asFloat();
		float3& scaleOffset = data.inputValue(DLInstancer::aScaleOffset, &status).asFloat3();

		transformData_.normalOffset = normalOffset;
		transformData_.translateOffset = translateOffset;
		transformData_.rotationOffset = rotationOffset;
		transformData_.uniformScaleOffset = uniformScaleOffset;
		transformData_.scaleOffset = scaleOffset;

		attributeDirty_[kOffsets] = false;
		recreateMatricies = true;
	}

	if (attributeDirty_[kRandoms] == true || transformData_.normalRandom.length() != numInstances_)
	{
		int seed = data.inputValue(DLInstancer::aNodeSeed).asInt();
		float maxNormalRandom = data.inputValue(DLInstancer::aNormalRandom, &status).asFloat();
		float3& maxTranslateRandom = data.inputValue(DLInstancer::aTranslateRandom, &status).asFloat3();
		float3& maxRotationRandom = data.inputValue(DLInstancer::aRotationRandom, &status).asFloat3();
		float maxUniformScaleRandom = data.inputValue(DLInstancer::aUniformScaleRandom, &status).asFloat();
		//float maxUniformScaleRandom = data.inputValue(DLInstancer::aUniformScaleOffset, &status).asFloat();
		float3& maxScaleRandom = data.inputValue(DLInstancer::aScaleRandom, &status).asFloat3();

		float normalRandom;
		MVector translateRandom;
		MVector rotationRandom;
		float uniformScaleRandom;
		MVector scaleRandom;

		transformData_.normalRandom.clear();
		transformData_.translateRandom.clear();
		transformData_.rotationRandom.clear();
		transformData_.uniformScaleRandom.clear();
		transformData_.scaleRandom.clear();

		for (unsigned int i = 0; i < numInstances_; ++i)
		{
			normalRandom = dlGenerateRandomValues((seed + i), maxNormalRandom, kNormal);
			translateRandom = dlGenerateRandomValues((seed + i), maxTranslateRandom, kTranslate);
			rotationRandom = dlGenerateRandomValues((seed + i), maxRotationRandom, kRotate);
			uniformScaleRandom = dlGenerateRandomValues((seed + i), maxUniformScaleRandom, kScale);
			scaleRandom = dlGenerateRandomValues((seed + i), maxScaleRandom, kScale);

			transformData_.normalRandom.append(normalRandom);
			transformData_.translateRandom.append(translateRandom);
			transformData_.rotationRandom.append(rotationRandom);
			transformData_.uniformScaleRandom.append(uniformScaleRandom);
			transformData_.scaleRandom.append(scaleRandom);

		}
		
		attributeDirty_[kRandoms] = false;
		recreateMatricies = true;
	}


	if (recreateOutMesh == true)
	{
		MGlobal::displayInfo("Recreate Mesh");
		status = dlCreateOutputMeshData(inputInstanceMeshData_, numInstances_, outputInstanceMeshData_);
		MObject generatedMesh = dlCreateMesh(outputInstanceMeshData_);
		hGeneratedMesh.set(generatedMesh);
	}

	if (recreateMatricies == true)
	{
		MGlobal::displayInfo("Recreate Matricies");
		//create new Matricies
		ouputTransformMatricies_.clear();
		ouputTransformMatricies_ = dlGenerateMatricies(transformData_);
	}


	//Deform Out Mesh
	hOutput.set(hGeneratedMesh.asMesh());
	dlDeformMesh(hOutput, ouputTransformMatricies_);
	
	data.setClean(plug);
	return MS::kSuccess;
}

MStatus DLInstancer::dlManualSetDependentsDirty(MDataBlock& data)
{
	MStatus status;
	MPlugArray plugArray;

	if (!data.isClean(aInstanceMesh))
	{
		MPlug plug(thisMObject(), aInstanceMesh);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aReferenceMesh))
	{
		MPlug plug(thisMObject(), aReferenceMesh);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aNormalOffset))
	{
		MPlug plug(thisMObject(), aNormalOffset);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aTranslateOffset))
	{
		MPlug plug(thisMObject(), aTranslateOffset);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aRotationOffset))
	{
		MPlug plug(thisMObject(), aRotationOffset);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aScaleOffset))
	{
		MPlug plug(thisMObject(), aScaleOffset);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aNormalRandom))
	{
		MPlug plug(thisMObject(), aNormalRandom);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aTranslateRandom))
	{
		MPlug plug(thisMObject(), aTranslateRandom);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aRotationRandom))
	{
		MPlug plug(thisMObject(), aRotationRandom);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aScaleRandom))
	{
		MPlug plug(thisMObject(), aScaleRandom);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aNodeSeed))
	{
		MPlug plug(thisMObject(), aNodeSeed);
		setDependentsDirty(plug, plugArray);
	}

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
	status = fnMesh.getVertexNormals(false, meshData.normals, MSpace::kWorld);
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

	numInstanceMeshPoints_ = inMeshData.numPoints;
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

	MPointArray points = transformData.referencePoints;
	MFloatVectorArray normals = transformData.referenceNormals;

	float normalOffset = transformData.normalOffset;
	MVector translateOffset = transformData.translateOffset;
	MVector rotationOffset = transformData.rotationOffset;
	float uniformScaleOffset = transformData.uniformScaleOffset;
	MVector scaleOffset = transformData.scaleOffset;

	MFloatArray normalRandom = transformData.normalRandom;
	MFloatVectorArray translateRandom = transformData.translateRandom;
	MFloatVectorArray rotationRandom = transformData.rotationRandom;
	MFloatArray uniformScaleRandom = transformData.uniformScaleRandom;
	MFloatVectorArray scaleRandom = transformData.scaleRandom;



	for (unsigned int i = 0; i < numInstances_; ++i)
	{
		MTransformationMatrix transformMatrix;

		double3 scale = { (scaleOffset[0] + scaleRandom[i].x + uniformScaleOffset + uniformScaleRandom[i]),
						  (scaleOffset[1] + scaleRandom[i].y + uniformScaleOffset + uniformScaleRandom[i]),
						  (scaleOffset[2] + scaleRandom[i].z + uniformScaleOffset + uniformScaleRandom[i]) };

		for (int i = 0; i < 3; ++i)
		{
			scale[i] <= 0 ? scale[i] = 0 : scale[i];
		}

		status = transformMatrix.setScale(scale, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		double3 rotation = { (rotationOffset[0] + rotationRandom[i].x),
							 (rotationOffset[1] + rotationRandom[i].y),
							 (rotationOffset[2] + rotationRandom[i].z) };
		status = transformMatrix.setRotation(rotation, MTransformationMatrix::kXYZ);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		MVector normal = normals[i] * (normalOffset + normalRandom[i]);
		MVector translation(points[i] + translateOffset + normal + translateRandom[i]);
		status = transformMatrix.setTranslation(translation, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		status = outMatrixArray.append(transformMatrix.asMatrix());
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}
	return outMatrixArray;
}

MStatus DLInstancer::dlDeformMesh(MDataHandle& meshDataHandle, MMatrixArray& matricies)
{
	MStatus status;
	MItGeometry itGeo(meshDataHandle, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (numInstances_ == 0)
	{
		return MS::kSuccess;
	}

	MPointArray allPoints;
	status = itGeo.allPositions(allPoints, MSpace::kWorld);

	MPoint point;
	unsigned int matrixIndex = 0;

	for (; !itGeo.isDone(); itGeo.next())
	{
		if (itGeo.index() % numInstanceMeshPoints_ == 0 && itGeo.index() != 0)
		{
			matrixIndex += 1;
		}

		point = allPoints[itGeo.index()];
		
		point *= ouputTransformMatricies_[matrixIndex];
		allPoints[itGeo.index()] = point;
	}

	status = itGeo.setAllPositions(allPoints, MSpace::kWorld);
	return MS::kSuccess;
}


