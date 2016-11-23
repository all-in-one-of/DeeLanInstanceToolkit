#include "DLInstancer.h"

//Type and ID
MTypeId DLInstancer::id(0x00000023);
const MString DLInstancer::nodeName = "dlInstancer";

//Input Attributes
MObject DLInstancer::aInstanceObject;
MObject DLInstancer::aInstanceMesh;
MObject DLInstancer::aInstanceMatrix;
MObject DLInstancer::aReferenceObject;
MObject DLInstancer::aReferenceMesh;
MObject DLInstancer::aReferenceMatrix;

MObject DLInstancer::aAlignToNormals;

MObject DLInstancer::aNormalOffset;
MObject DLInstancer::aTranslate;
MObject DLInstancer::aRotate;
MObject DLInstancer::aUniformScale;
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

	//Output Attributes
	aOutMesh = tAttr.create("outMesh", "oMesh", MFnData::kMesh);
	tAttr.setStorable(false);
	tAttr.setWritable(false);
	addAttribute(aOutMesh);

	//Input Attributes
	aInstanceObject = cAttr.create("instanceObject", "iObj");

	aInstanceMesh = tAttr.create("instanceMesh", "iMesh", MFnData::kMesh);
	tAttr.setReadable(false);

	aInstanceMatrix = mAttr.create("instanceMatrix", "iMatrix");
	mAttr.setDefault(MMatrix::identity);
	//mAttr.setDisconnectBehavior(MFnAttribute::kReset)

	cAttr.addChild(aInstanceMesh);
	cAttr.addChild(aInstanceMatrix);

	addAttribute(aInstanceObject);
	attributeAffects(aInstanceMesh, aOutMesh);
	attributeAffects(aInstanceMatrix, aOutMesh);




	aReferenceObject = cAttr.create("referenceObject", "rObj");

	aReferenceMesh = tAttr.create("referenceMesh", "rMesh", MFnData::kMesh);
	tAttr.setReadable(false);

	aReferenceMatrix = mAttr.create("referenceMatrix", "rMatrix");
	mAttr.setDefault(MMatrix::identity);
	//mAttr.setDisconnectBehavior(MFnAttribute::kReset)

	cAttr.addChild(aReferenceMesh);
	cAttr.addChild(aReferenceMatrix);

	addAttribute(aReferenceObject);
	attributeAffects(aReferenceMesh, aOutMesh);
	attributeAffects(aReferenceMatrix, aOutMesh);







	aAlignToNormals = nAttr.create("alignToNormals", "alignNormals", MFnNumericData::kBoolean, true);
	nAttr.setKeyable(true);
	addAttribute(aAlignToNormals);
	attributeAffects(aAlignToNormals, aOutMesh);

	aNormalOffset = nAttr.create("normalOffset", "nOff", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aNormalOffset);
	attributeAffects(aNormalOffset, aOutMesh);
	
	aTranslate = nAttr.create("translateOffset", "tOff", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aTranslate);
	attributeAffects(aTranslate, aOutMesh);

	aRotate = nAttr.create("rotationOffset", "rOff", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aRotate);
	attributeAffects(aRotate, aOutMesh);

	aUniformScale = nAttr.create("uniformScaleOffset", "usOff", MFnNumericData::kFloat, 1.0f);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	addAttribute(aUniformScale);
	attributeAffects(aUniformScale, aOutMesh);

	aScaleOffset = nAttr.create("scaleOffset", "sOff", MFnNumericData::k3Float, (0.0f, 0.0f, 0.0f));
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f, 0.0f, 0.0f);
	addAttribute(aScaleOffset);
	attributeAffects(aScaleOffset, aOutMesh);

	aNormalRandom = nAttr.create("normalRandom", "nRand", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	addAttribute(aNormalRandom);
	attributeAffects(aNormalRandom, aOutMesh);

	aTranslateRandom = nAttr.create("translateRandom", "tRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f, 0.0f, 0.0f);
	addAttribute(aTranslateRandom);
	attributeAffects(aTranslateRandom, aOutMesh);

	aRotationRandom = nAttr.create("rotationRandom", "rRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f, 0.0f, 0.0f);
	addAttribute(aRotationRandom);
	attributeAffects(aRotationRandom, aOutMesh);

	aUniformScaleRandom = nAttr.create("uniformScaleRandom", "usRand", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	addAttribute(aUniformScaleRandom);
	attributeAffects(aUniformScaleRandom, aOutMesh);

	aScaleRandom = nAttr.create("scaleRandom", "sRand", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f, 0.0f, 0.0f);
	addAttribute(aScaleRandom);
	attributeAffects(aScaleRandom, aOutMesh);

	aNodeSeed = nAttr.create("nodeSeed", "nSeed", MFnNumericData::kInt);
	nAttr.setKeyable(true);
	addAttribute(aNodeSeed);
	attributeAffects(aNodeSeed, aOutMesh);

	aGeneratedMesh = tAttr.create("generatedMesh", "gMesh", MFnData::kMesh);
	tAttr.setConnectable(false);
	tAttr.setHidden(true);
	addAttribute(aGeneratedMesh);



	return MS::kSuccess;
}

MStatus DLInstancer::setDependentsDirty(const MPlug &plug, MPlugArray &plugArray)
{
	MStatus status;

	//MGlobal::displayInfo("SET DEPENDENTS DIRTY CALLED!!!"); //DEBUGGING

	if (plug == aInstanceMesh)
	{
		attributeDirty_[kInstanceMesh] = true;
	}
	else if (plug == aReferenceMesh)
	{
		attributeDirty_[kReferenceMesh] = true;
	}
	else if (plug == aInstanceMatrix || plug == aReferenceMatrix)
	{
		//MGlobal::displayInfo("MATRIX STUFF YO!!!!!"); //DEBUGGING
		attributeDirty_[kTransMatrix] = true;
	}
	else if (plug == aNormalOffset || plug.parent() == aTranslate ||
			 plug.parent() == aRotate || plug.parent() == aScaleOffset ||
			 plug == aUniformScale)
	{
		attributeDirty_[kOffsets] = true;
	}
	else if (plug == aNormalRandom || plug.parent() == aTranslateRandom || 
			 plug.parent() == aRotationRandom || plug.parent() == aScaleRandom ||
			 plug == aUniformScaleRandom || plug == aNodeSeed)
	{
		attributeDirty_[kRandoms] = true;
	}
	else if (plug == aAlignToNormals)
	{
		attributeDirty_[kAlignment] = true;
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

	//MGlobal::displayInfo("COMPUTE CALLED!!!"); //DEBUGGING

	//Supporting Timeline Playback
	MAnimControl anim;
	MTime curTime = anim.currentTime();
	if (curTime != prevTime_)
	{
		prevTime_ = curTime;
		//MGlobal::displayInfo("Time Is Different"); //DEBUGGING
		if (setDependentsDirtyCalled_ == false)
		{
			//MGlobal::displayInfo("setDepDirty not Called"); //DEBUGGING

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
		//MGlobal::displayInfo("Instance Mesh"); //DEBUGGING
		MObject instanceMesh = data.inputValue(DLInstancer::aInstanceMesh, &status).asMesh();
		CHECK_MSTATUS_AND_RETURN_IT(status);





		status = dlGetMeshData(instanceMesh, inputMeshData_);
		CHECK_MSTATUS_AND_RETURN_IT(status);





		attributeDirty_[kInstanceMesh] = false;
		recreateOutMesh = true;
	}

	if (attributeDirty_[kReferenceMesh] == true)
	{
		//MGlobal::displayInfo("Reference Mesh"); //DEBUGGING
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
		transformData_.referencePoints.clear();
		transformData_.referenceNormals.clear();
		transformData_.normalAlignmentMatricies.clear();

		transformData_.referencePoints = points;
		transformData_.referenceNormals = normals;

		
		for (unsigned int i = 0; i < normals.length(); ++i)
		{
			MMatrix alignMatrix = dlGenerateNormalAlignmentMatrix(normals[i]);
			transformData_.normalAlignmentMatricies.append(alignMatrix);

		}



		//calculate normal rotations and store in transform data
		attributeDirty_[kReferenceMesh] = false;
		recreateMatricies = true;
	}

	if (attributeDirty_[kOffsets] == true)
	{
		//MGlobal::displayInfo("Offsets"); //DEBUGGING




		float normalOffset = data.inputValue(DLInstancer::aNormalOffset, &status).asFloat();
		float3& translateOffset = data.inputValue(DLInstancer::aTranslate, &status).asFloat3();
		float3& rotationOffset = data.inputValue(DLInstancer::aRotate, &status).asFloat3();
		float uniformScaleOffset = data.inputValue(DLInstancer::aUniformScale, &status).asFloat();
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
		//MGlobal::displayInfo("Randoms"); //DEBUGGING




		int seed = data.inputValue(DLInstancer::aNodeSeed).asInt();
		float maxNormalRandom = data.inputValue(DLInstancer::aNormalRandom, &status).asFloat();
		float3& maxTranslateRandom = data.inputValue(DLInstancer::aTranslateRandom, &status).asFloat3();
		float3& maxRotationRandom = data.inputValue(DLInstancer::aRotationRandom, &status).asFloat3();
		float maxUniformScaleRandom = data.inputValue(DLInstancer::aUniformScaleRandom, &status).asFloat();
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
			normalRandom = DLCommon::dlGenerateRandomValues((seed + i), maxNormalRandom, DLCommon::kNormal);
			translateRandom = DLCommon::dlGenerateRandomValues((seed + i), maxTranslateRandom, DLCommon::kTranslate);
			rotationRandom = DLCommon::dlGenerateRandomValues((seed + i), maxRotationRandom, DLCommon::kRotate);
			uniformScaleRandom = DLCommon::dlGenerateRandomValues((seed + i), maxUniformScaleRandom, DLCommon::kScale);
			scaleRandom = DLCommon::dlGenerateRandomValues((seed + i), maxScaleRandom, DLCommon::kScale);

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
		//MGlobal::displayInfo("Recreate Mesh"); //DEBUGGING




		status = dlCreateOutputMeshData(inputMeshData_, numInstances_, outputMeshData_);
		MObject generatedMesh = dlCreateMesh(outputMeshData_);
		hGeneratedMesh.set(generatedMesh);



	}

		
	if (recreateMatricies == true || attributeDirty_[kAlignment] == true || 
									 attributeDirty_[kTransMatrix] == true)
	{



		//MGlobal::displayInfo("Recreate Matricies"); //DEBUGGING
		bool alignToNormals = data.inputValue(DLInstancer::aAlignToNormals).asBool();
		instanceMatrix_ = data.inputValue(DLInstancer::aInstanceMatrix, &status).asMatrix();
		CHECK_MSTATUS_AND_RETURN_IT(status);
		referenceMatrix_ = data.inputValue(DLInstancer::aReferenceMatrix, &status).asMatrix();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		//create new Matricies
		ouputTransformMatricies_.clear();
		ouputTransformMatricies_ = dlGenerateInstanceDeformMatricies(transformData_, alignToNormals);




		attributeDirty_[kAlignment] = false;
		attributeDirty_[kTransMatrix] = false;
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
	if (!data.isClean(aTranslate))
	{
		MPlug plug(thisMObject(), aTranslate);
		setDependentsDirty(plug, plugArray);
	}
	if (!data.isClean(aRotate))
	{
		MPlug plug(thisMObject(), aRotate);
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
	meshData.edgeSmoothing.clear();
	meshData.uArray.clear();
	meshData.vArray.clear();
	meshData.uvCounts.clear();
	meshData.uvIDs.clear();



	MFnMesh fnMesh(mesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MItMeshPolygon itPoly(mesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MObject meshCopy(mesh);
	MItMeshEdge itEdge(meshCopy, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MItMeshFaceVertex itFaceVert(mesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	meshData.numPoints = fnMesh.numVertices();
	numInstanceMeshPoints_ = fnMesh.numVertices();
	meshData.numPolys = fnMesh.numPolygons();


	status = fnMesh.getPoints(meshData.pointArray, MSpace::kWorld);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	for (; !itEdge.isDone(); itEdge.next())
	{
		meshData.edgeSmoothing.push_back(itEdge.isSmooth());
	}
	

	MStringArray uvSets;
	fnMesh.getUVSetNames(uvSets);
	MString map = uvSets[0];
	status = fnMesh.getUVs(meshData.uArray, meshData.vArray, &map);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	fnMesh.getAssignedUVs(meshData.uvCounts, meshData.uvIDs, &map);

	//MString tempString;
	//tempString = meshData.uArray.length();
	//MGlobal::displayInfo(tempString);

	for (; !itPoly.isDone(); itPoly.next())
	{
		int numPolyVerts = itPoly.polygonVertexCount(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = meshData.polyCounts.append(numPolyVerts);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MIntArray polyVertIDs;
		status = itPoly.getVertices(polyVertIDs);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		for (unsigned int i = 0; i < polyVertIDs.length(); ++i)
		{
			status = meshData.polyConnects.append(polyVertIDs[i]);
			CHECK_MSTATUS_AND_RETURN_IT(status);
		}
	}
	return MS::kSuccess;
}

MStatus DLInstancer::dlCreateOutputMeshData(const DLMeshData & inMeshData, unsigned int numCopies, DLMeshData & outMeshData, bool clearData)
{
	MTimer dlCreateOutputMeshDataTimer; //PERFORMANCE TIMING //
	dlCreateOutputMeshDataTimer.beginTimer(); //PERFORMANCE TIMING //


	MStatus status;

	if (clearData)
	{
		outMeshData.pointArray.clear();
		outMeshData.polyCounts.clear();
		outMeshData.polyConnects.clear();
		outMeshData.edgeSmoothing.clear();
		outMeshData.uArray.clear();
		outMeshData.vArray.clear();
		outMeshData.uvIDs.clear();
		outMeshData.uvCounts.clear();
	}

	
	outMeshData.numPoints = inMeshData.numPoints * numCopies;
	outMeshData.numPolys = inMeshData.numPolys * numCopies;

	for (unsigned int i = 0; i < numCopies; ++i)
	{

		MTimer appendArrayDataTimer; //PERFORMANCE TIMING //
		appendArrayDataTimer.beginTimer(); //PERFORMANCE TIMING //


		outMeshData.appendArrayData(inMeshData);

		appendArrayDataTimer.endTimer(); //PERFORMANCE TIMING //
		MString tempString2("appendArrayData COMPUTE TIME: "); //PERFORMANCE TIMING //
		tempString2 += appendArrayDataTimer.elapsedTime(); //PERFORMANCE TIMING //
		MGlobal::displayInfo(tempString2); //PERFORMANCE TIMING //
		
	}

	dlCreateOutputMeshDataTimer.endTimer(); //PERFORMANCE TIMING //
	MString tempString("dlCreateOutputMeshData COMPUTE TIME: "); //PERFORMANCE TIMING //
	tempString += dlCreateOutputMeshDataTimer.elapsedTime(); //PERFORMANCE TIMING //
	MGlobal::displayInfo(tempString); //PERFORMANCE TIMING //

	return MS::kSuccess;
}

MObject DLInstancer::dlCreateMesh(const DLMeshData& meshData)
{

	MTimer dlCreateMeshTimer; //PERFORMANCE TIMING //
	dlCreateMeshTimer.beginTimer(); //PERFORMANCE TIMING //


	MStatus status;
	MFnMeshData  dataFn;
	MObject dataWrapper = dataFn.create();
	MFnMesh generator;

	
	MTimer generateMeshTimer; //PERFORMANCE TIMING //
	generateMeshTimer.beginTimer(); //PERFORMANCE TIMING //


	MObject outMesh = generator.create(meshData.numPoints, meshData.numPolys, 
									   meshData.pointArray, meshData.polyCounts, 
									   meshData.polyConnects, meshData.uArray, 
									   meshData.vArray, dataWrapper, &status);


	generateMeshTimer.endTimer(); //PERFORMANCE TIMING //
	MString tempString2("generateMesh COMPUTE TIME: "); //PERFORMANCE TIMING //
	tempString2 += generateMeshTimer.elapsedTime(); //PERFORMANCE TIMING //
	MGlobal::displayInfo(tempString2); //PERFORMANCE TIMING //


	MItMeshEdge itEdge(dataWrapper);

	for (; !itEdge.isDone(); itEdge.next())
	{
		itEdge.setSmoothing(meshData.edgeSmoothing[itEdge.index()]);
	}

	MFnMesh fnOutMesh(dataWrapper);
	MString map = "map1";
	status = fnOutMesh.assignUVs(meshData.uvCounts, meshData.uvIDs, &map);
	if (status != MS::kSuccess)
	{
		MGlobal::displayError(status.errorString());
		MGlobal::displayError("Problem Assigning UVs");
	}


	if (status != MS::kSuccess)
	{
		MGlobal::displayError(status.errorString());
		MGlobal::displayError("Problem Creating Mesh. Returning Null Mesh");
		MFnMeshData fnNullMesh;
		return fnNullMesh.create();
	}


	dlCreateMeshTimer.endTimer(); //PERFORMANCE TIMING //
	MString tempString("dlCreateMesh COMPUTE TIME: "); //PERFORMANCE TIMING //
	tempString += dlCreateMeshTimer.elapsedTime(); //PERFORMANCE TIMING //
	MGlobal::displayInfo(tempString); //PERFORMANCE TIMING //


	return dataWrapper;
}

MMatrix DLInstancer::dlGenerateNormalAlignmentMatrix(MVector direction)
{
	MStatus status;
	MVector sideVec(1, 0, 0);

	MVector y = direction;
	y.normalize();
	MVector z = sideVec ^ direction;
	z.normalize();
	MVector x = direction ^ z;
	x.normalize();


	MMatrix matrix;

	matrix[0][0] = x.x; matrix[0][1] = x.y; matrix[0][2] = x.z; matrix[0][3] = 0.0;
	matrix[1][0] = y.x; matrix[1][1] = y.y; matrix[1][2] = y.z; matrix[1][3] = 0.0;
	matrix[2][0] = z.x; matrix[2][1] = z.y; matrix[2][2] = z.z; matrix[2][3] = 0.0;
	matrix[3][0] = 0.0; matrix[3][1] = 0.0; matrix[3][2] = 0.1; matrix[3][3] = 1.0;

	return matrix;
}

MMatrixArray DLInstancer::dlGenerateInstanceDeformMatricies(const DLTransformData& transformData, bool alignNormals)
{
	MStatus status;

	MMatrixArray outMatrixArray;

	MMatrixArray normalMatricies = transformData_.normalAlignmentMatricies;

	MPointArray points = transformData.referencePoints;
	MFloatVectorArray normals = transformData.referenceNormals;

	
	double matrixConversionArray[4][4];
	referenceMatrix_.get(matrixConversionArray);
	MFloatMatrix refMatAsFloatMat(matrixConversionArray);

	MTransformationMatrix refMatAsTransMat = referenceMatrix_;
	double3 tempScale = { 1,1,1 };
	refMatAsTransMat.setScale(tempScale, MSpace::kWorld);
	for (unsigned int i = 0; i < numInstances_; ++i)
	{
		points[i] *= referenceMatrix_;
		normals[i] *= refMatAsFloatMat;
		normalMatricies[i] *= refMatAsTransMat.asMatrix();
	}

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

	
	MVector instanceTranslation;
	for (unsigned int i = 0; i < numInstances_; ++i)
	{
		MTransformationMatrix transformMatrix;

		if (alignNormals)
		{
			transformMatrix = normalMatricies[i];
			instanceTranslation = instanceMatrix_.getTranslation(MSpace::kWorld) * normalMatricies[i];
		}
		else
		{
			transformMatrix = MMatrix::identity;
			instanceTranslation = instanceMatrix_.getTranslation(MSpace::kWorld);
		}

		MVector uniSclOff({ uniformScaleOffset , uniformScaleOffset , uniformScaleOffset });
		MVector uniSclRnd({ uniformScaleRandom[i] , uniformScaleRandom[i] , uniformScaleRandom[i] });
		
		double3 instSclAry;
		instanceMatrix_.getScale(instSclAry, MSpace::kWorld);
		MVector instanceScale(instSclAry);

		MVector scaleVec(scaleOffset + scaleRandom[i] + uniSclOff + uniSclRnd);
		scaleVec.x *= instanceScale.x;
		scaleVec.y *= instanceScale.y;
		scaleVec.z *= instanceScale.z;
		double3 scale = { scaleVec.x, scaleVec.y, scaleVec.z };

		for (int i = 0; i < 3; ++i)
		{
			scale[i] <= 0 ? scale[i] = 0 : scale[i];
		}
		status = transformMatrix.setScale(scale, MSpace::kWorld);
		


		double3 instRotAry;
		MTransformationMatrix::RotationOrder ro;
		instanceMatrix_.getRotation(instRotAry, ro);
		MVector instanceRotation(instRotAry);

		MVector rotationVec(rotationOffset + rotationRandom[i] + instanceRotation);
		double3 rotation = { rotationVec.x, rotationVec.y,rotationVec.z };
		status = transformMatrix.addRotation(rotation, MTransformationMatrix::kXYZ, MSpace::kTransform);
		

		

		MVector normalTranslation = normals[i] * (normalOffset + normalRandom[i]);
		MVector translation(points[i] + translateOffset + normalTranslation + translateRandom[i] + instanceTranslation);
		status = transformMatrix.setTranslation(translation, MSpace::kWorld);
		

		MMatrix outMatrix(transformMatrix.asMatrix());
		status = outMatrixArray.append(outMatrix);
		

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


