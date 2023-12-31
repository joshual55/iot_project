// Source cited:
// https://aws.plainenglish.io/facial-recognition-using-amazon-rekognition-lambda-and-javascript-6be1349a41a4

const AWS = require('aws-sdk');
const rekognition = new AWS.Rekognition();
exports.handler = async (event) => {
    // Form the response to be returned
    let response = {
        "data": "",
        "message": "",
        "error": ""
    };
    
    // Create a collection
    try {
        // Name the collection uniquely
        var params = {
            "CollectionId": "faces-collection"
        };
         
        const data = await rekognition.createCollection(params).promise();
        
        response.data = data;
        response.message = "Collection Created Successfully";
        response.error = null;
    } catch (e) {
        response.data = null;
        response.message = "Collection Created Failed";
        response.error = e;
    }
    return response;
};