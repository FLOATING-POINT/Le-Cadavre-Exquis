#include "openNIWrapper.h"

openNIWrapper * openNIWrapperPtr;

static void XN_CALLBACK_TYPE ONI_NewUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{

	openNIWrapperPtr->_AddPlayerEvent((unsigned int)user);
};
static void XN_CALLBACK_TYPE ONI_LostUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	openNIWrapperPtr->_RemovePlayerEvent((unsigned int)user);
};

static void XN_CALLBACK_TYPE ONI_PoseDetected(xn::PoseDetectionCapability& pose, const XnChar* strPose, XnUserID user, void* cxt)
{
	openNIWrapperPtr->_PoseDetectedEvent((unsigned int)user);
}

static void XN_CALLBACK_TYPE ONI_CalibrationStarted(xn::SkeletonCapability& skeleton, XnUserID user, void* cxt)
{
	printf("ONI :: Calibration started\n");
	openNIWrapperPtr->_CalibrationStartedEvent((unsigned int)user);
}

static void XN_CALLBACK_TYPE ONI_CalibrationEnded(xn::SkeletonCapability& skeleton, XnUserID user, XnBool bSuccess, void* cxt)
{
	openNIWrapperPtr->_CalibrationEndedEvent((unsigned int)user, bSuccess);

}

//--------------------------------------------------------------

openNIWrapper::openNIWrapper()
{
    openNIWrapperPtr = this;
    debug = true;
}

openNIWrapper::~openNIWrapper()
{
    context->Shutdown();
    free(context);
    /*
    delete videoPixels;
    delete depthRawPixels;
    delete depthPixels;
    */
}

//--------------------------------------------------------------
void openNIWrapper::setup()
{
    ofSetWindowPosition(ofGetScreenWidth() - ofGetWidth() - 8, 25);

    context = new Context;

    doDrawLimb = true;

    scale   = 1.0f;

    nRetVal             = XN_STATUS_OK;
    offsetX             = 0;
    offsetY             = 0;
    displayWidth        = 0;
    displayHeight       = 0;
    offsetYSkeletal     = 0;
    offsetYSkeletalAmt  = 30;

	nRetVal = context->InitFromXmlFile(SAMPLE_XML_PATH, &errors);

	if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
		OF_EXIT_APP(0);
	}
	else if (nRetVal != XN_STATUS_OK)
	{
		printf("OpenNI settings data/openNI/SamplesConfig.xml Open failed: %s\n", xnGetStatusString(nRetVal));
		OF_EXIT_APP(0);
	}

	nRetVal = context->FindExistingNode(XN_NODE_TYPE_DEPTH, depthGenerator);
	CHECK_RC(nRetVal, "Find depth generator");

	nRetVal = context->FindExistingNode(XN_NODE_TYPE_IMAGE, imageGenerator);
	CHECK_RC(nRetVal, "Find image generator");

    printf("starting user generator...\n");
    nRetVal = context->FindExistingNode(XN_NODE_TYPE_USER, userGenerator);
    CHECK_RC(nRetVal, "Find user generator");
    XnCallbackHandle hUserCBs;
    userGenerator.RegisterUserCallbacks(ONI_NewUser, ONI_LostUser, NULL, hUserCBs);

    printf("starting skeleton tracker...\n");
    if (!userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON) || !userGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
    {
        printf("User generator doesn't support either skeleton or pose detection.\n");
        OF_EXIT_APP(0);
    }
    userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    XnCallbackHandle hCalibrationCBs, hPoseCBs;
    userGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(ONI_CalibrationStarted, ONI_CalibrationEnded, NULL, hCalibrationCBs);
    userGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(ONI_PoseDetected, NULL, NULL, hPoseCBs);

    videoPixels     = new unsigned char[width * height * 3];
    depthRawPixels  = new unsigned short[width * height];
    depthPixels     = new unsigned char[width * height];

    rgbImage.allocate(width, height, OF_IMAGE_COLOR);
    depthImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void openNIWrapper::update(){

    float wRatio, hRatio;

    wRatio = ofGetWidth() / (float)width;
    hRatio = ofGetHeight() / (float)height;

   // scale = MIN( wRatio, hRatio );
    scale = MAX( wRatio, hRatio );


    displayWidth    = 640*scale;
    displayHeight   = 480*scale;

    offsetX         = (ofGetWidth()-displayWidth)*.5;
    offsetY         = (ofGetHeight()-displayHeight)*.5;
    offsetYSkeletal = offsetY + (offsetYSkeletalAmt*scale);



    nRetVal = context->WaitOneUpdateAll(depthGenerator);

    if (nRetVal != XN_STATUS_OK)
    {
        printf("UpdateData failed: %s\n", xnGetStatusString(nRetVal));
        return;
    }

    depthGenerator.GetMetaData(depthMetaData);
    imageGenerator.GetMetaData(imageMetaData);
    userGenerator.GetUserPixels(0, sceneMetaData);

    videoPixels = (unsigned char*)imageMetaData.Data();
    depthRawPixels = (unsigned short*)depthMetaData.Data();

    for (int i =0 ; i < width * height; i++)
    {
        if (depthRawPixels[i] != 0)
            depthPixels[i] = 255 - depthRawPixels[i] * 255 / 8192;
        else depthPixels[i] = 0;
    }

    rgbImage.setFromPixels(videoPixels, width, height, OF_IMAGE_COLOR);
    depthImage.setFromPixels(depthPixels, width, height, OF_IMAGE_GRAYSCALE);
}


//--------------------------------------------------------------
void openNIWrapper::draw()
{
    ofBackground(0,0,0);
    ofSetColor(255, 255, 255);

    //depthImage.draw(0, 0, 640*scale, 480*scale);
    rgbImage.draw(offsetX, offsetY, displayWidth, displayHeight);


    if(doDrawLimb){

        XnUserID aUsers[15];
        XnUInt16 nUsers = 15;

        userGenerator.GetUsers(aUsers, nUsers);

        for (int i = 0; i < nUsers; ++i){

            XnPoint3D com;
            userGenerator.GetCoM(aUsers[i], com);
            depthGenerator.ConvertRealWorldToProjective(1, &com, &com);

            if(debug){
                ofSetColor(0, 255, 255);
                ofFill();
                ofCircle(com.X*scale, com.Y*scale, 5);
                ofDrawBitmapString("id: " + ofToString((int)aUsers[i]), com.X*scale, com.Y*scale);
            }

            if (userGenerator.GetSkeletonCap().IsTracking(aUsers[i])){

                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_HEAD)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_NECK)));

                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_NECK)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_SHOULDER)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_SHOULDER)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_ELBOW)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_ELBOW)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_HAND)));

                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_NECK)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_SHOULDER)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_SHOULDER)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_ELBOW)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_ELBOW)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_HAND)));

                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_SHOULDER)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_TORSO)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_SHOULDER)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_TORSO)));

                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_TORSO)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_HIP)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_HIP)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_KNEE)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_KNEE)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_FOOT)));

                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_TORSO)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_HIP)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_HIP)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_KNEE)));
                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_KNEE)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_FOOT)));

                drawLimb(getScreenPoint(getJoint(aUsers[i], XN_SKEL_LEFT_HIP)), getScreenPoint(getJoint(aUsers[i], XN_SKEL_RIGHT_HIP)));

                if(i == 0){
                    leftHandUser1Pnt     = getScreenPoint(getJoint(aUsers[0], XN_SKEL_LEFT_HAND));
                    rightHandUser1Pnt    = getScreenPoint(getJoint(aUsers[0], XN_SKEL_RIGHT_HAND));

                    leftHandUser1Pnt.x *= scale;
                    leftHandUser1Pnt.y *= scale;

                    rightHandUser1Pnt.x *= scale;
                    rightHandUser1Pnt.y *= scale;
                }
                if(i == 1){

                    leftHandUser2Pnt     = getScreenPoint(getJoint(aUsers[1], XN_SKEL_LEFT_HAND));
                    rightHandUser2Pnt    = getScreenPoint(getJoint(aUsers[1], XN_SKEL_RIGHT_HAND));

                    leftHandUser2Pnt.x *= scale;
                    leftHandUser2Pnt.y *= scale;

                    rightHandUser2Pnt.x *= scale;
                    rightHandUser2Pnt.y *= scale;
                }
            }
        }

        if(nUsers == 0){

            leftHandUser1Pnt.x = 0;
            leftHandUser1Pnt.y = 0;

            rightHandUser1Pnt.x = 0;
            rightHandUser1Pnt.y = 0;


            leftHandUser2Pnt.x = 0;
            leftHandUser2Pnt.y = 0;

            rightHandUser2Pnt.x = 0;
            rightHandUser2Pnt.y = 0;

        }

        if(nUsers == 1){

            leftHandUser2Pnt.x = 0;
            leftHandUser2Pnt.y = 0;

            rightHandUser2Pnt.x = 0;
            rightHandUser2Pnt.y = 0;

        }
    }

}

void openNIWrapper::drawLimb(ofPoint a, ofPoint b)
{
    glLineWidth(3);
        ofSetColor(255, 0, 0);
        ofLine(offsetX + a.x*scale, offsetYSkeletal + a.y*scale, offsetX + b.x*scale, offsetYSkeletal + b.y*scale);
    glLineWidth(1);
}

ofPoint openNIWrapper::getJoint(int userID, XnSkeletonJoint jointName)
{
    if (!userGenerator.GetSkeletonCap().IsTracking(userID))
	{
		printf("not tracked!\n");
		return ofPoint();
	}
	XnSkeletonJointPosition joint;
    userGenerator.GetSkeletonCap().GetSkeletonJointPosition(userID, jointName, joint);

    return ofPoint(joint.position.X, joint.position.Y, joint.position.Z);
}

ofPoint openNIWrapper::getScreenPoint(ofPoint p3d)
{
    XnPoint3D pt[1];
	pt[0].X = p3d.x;
	pt[0].Y = p3d.y;
	pt[0].Z = p3d.z;
	depthGenerator.ConvertRealWorldToProjective(1, pt, pt);
    return ofPoint(pt[0].X, pt[0].Y);

}

void openNIWrapper::_AddPlayerEvent(int playerID)
{
    printf("ONI :: New User id: %i\n", playerID);
    userGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", playerID);

}

void openNIWrapper::_RemovePlayerEvent(int playerID)
{
    printf("ONI :: Lost User id: %i\n", playerID);
}

void openNIWrapper::_PoseDetectedEvent(int playerID)
{
    printf("ONI :: Pose Detected: id: %i\n", playerID);
    userGenerator.GetSkeletonCap().RequestCalibration(playerID, TRUE);
	userGenerator.GetPoseDetectionCap().StopPoseDetection(playerID);
}

void openNIWrapper::_CalibrationStartedEvent(int playerID)
{
    printf("ONI :: Calibration started: id: %i\n", playerID);
}

void openNIWrapper::_CalibrationEndedEvent(int playerID, bool success)
{
    printf("ONI :: Calibration ended: id: %i\n", playerID);
    if (success)
    {
        userGenerator.GetSkeletonCap().SaveCalibrationData(playerID, 0);
        userGenerator.GetSkeletonCap().StartTracking(playerID);
        userGenerator.GetPoseDetectionCap().StopPoseDetection(playerID);
    }
    else
    {
        userGenerator.GetPoseDetectionCap().StopPoseDetection(playerID);
        userGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", playerID);
    }
}


//--------------------------------------------------------------
void openNIWrapper::keyPressed  (int key)
{

}

//--------------------------------------------------------------
void openNIWrapper::keyReleased(int key)
{

}

//--------------------------------------------------------------
void openNIWrapper::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void openNIWrapper::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void openNIWrapper::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void openNIWrapper::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void openNIWrapper::windowResized(int w, int h)
{

}

