// Source cited:
// https://aws.plainenglish.io/facial-recognition-using-amazon-rekognition-lambda-and-javascript-6be1349a41a4

const AWS = require('aws-sdk');
const rekognition = new AWS.Rekognition();

exports.handler = async (event) => {
    // Form the response to be returned
    let response = {
        data: "",
        message: "",
        error: ""
    };
    
    // Add a face to the collection
    try {
        // Encoded image without the 'data:image/png;base64,' text
        const parsedJson = JSON.parse(event.body);
        const encodedImage = parsedJson.imgdata;
        // const encodedImage = event.imgdata;
        
        // Convert to buffer
        const decodedImage = Buffer.from(encodedImage, 'base64');
        // Params for rekoginition api
        let params = {
            // Collection name to store the face
            CollectionId: "faces-collection", 
            DetectionAttributes: [
                "DEFAULT"
            ], 
            // ExternalImageId: "myphotoid", 
            Image: {
                Bytes: decodedImage
            }
        };
        
        const data = await rekognition.indexFaces(params).promise();
        
        response.data = data;
        response.message = "Face Added Successfully!!"
        response.error = null;
        
        return {
            statusCode: 200,
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(response)
        }
    } catch (e) {
        response.data = null;
        response.message = "Failed to Add Face Data!!"
        response.error = e;
        
        return {
            statusCode: 404,
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(response)
        }
    }
};