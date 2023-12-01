// Source cited:
// https://aws.plainenglish.io/facial-recognition-using-amazon-rekognition-lambda-and-javascript-6be1349a41a4

const AWS = require('aws-sdk');
const rekognition = new AWS.Rekognition();

exports.handler = async (event) => {
    
    // Response
    const response = {
        data: null,
        message: '',
    };
    
    // Facial search
    
    // Get the encoded image without the 'data:image/png;base64,' text
    const parsedJson = JSON.parse(event.body);
    const encodedImage = parsedJson.imgdata;
    // const encodedImage = event.imgdata;
    
    // Convert to buffer
    const decodedImage = Buffer.from(encodedImage, 'base64');
    // Params for rekoginition api
    var params = {
        // Collection name to store the face
        CollectionId: "faces-collection", 
        
        // Minimum match confidence score
        FaceMatchThreshold: 90, 
            Image: {
            Bytes: decodedImage
        },
        
        // Max no of faces to return
        MaxFaces: 1
    };
    
    // Call rekognition api to search the input face
    const data = await rekognition.searchFacesByImage(params).promise();
    
    // See if any matches were found
    const faceMatches = data['FaceMatches'];
    if (faceMatches.length == 0) {
        response.data = null;
        response.message = "Face recognition failed"
        response.error = null;
      
        return {
            statusCode: 404,
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(response)
        }
    } else {
        response.data = data;
        response.message = "Face recognized successfully!!"
        response.error = null;
      
        return {
            statusCode: 200,
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(response)
        }
    }
};
