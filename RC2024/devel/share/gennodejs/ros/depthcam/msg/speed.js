// Auto-generated. Do not edit!

// (in-package depthcam.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class speed {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.X_speed = null;
      this.Y_speed = null;
      this.Z_speed = null;
    }
    else {
      if (initObj.hasOwnProperty('X_speed')) {
        this.X_speed = initObj.X_speed
      }
      else {
        this.X_speed = 0.0;
      }
      if (initObj.hasOwnProperty('Y_speed')) {
        this.Y_speed = initObj.Y_speed
      }
      else {
        this.Y_speed = 0.0;
      }
      if (initObj.hasOwnProperty('Z_speed')) {
        this.Z_speed = initObj.Z_speed
      }
      else {
        this.Z_speed = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type speed
    // Serialize message field [X_speed]
    bufferOffset = _serializer.float32(obj.X_speed, buffer, bufferOffset);
    // Serialize message field [Y_speed]
    bufferOffset = _serializer.float32(obj.Y_speed, buffer, bufferOffset);
    // Serialize message field [Z_speed]
    bufferOffset = _serializer.float32(obj.Z_speed, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type speed
    let len;
    let data = new speed(null);
    // Deserialize message field [X_speed]
    data.X_speed = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [Y_speed]
    data.Y_speed = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [Z_speed]
    data.Z_speed = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 12;
  }

  static datatype() {
    // Returns string type for a message object
    return 'depthcam/speed';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '7b206176458d8484913bad2dea5ffce8';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 X_speed
    float32 Y_speed
    float32 Z_speed
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new speed(null);
    if (msg.X_speed !== undefined) {
      resolved.X_speed = msg.X_speed;
    }
    else {
      resolved.X_speed = 0.0
    }

    if (msg.Y_speed !== undefined) {
      resolved.Y_speed = msg.Y_speed;
    }
    else {
      resolved.Y_speed = 0.0
    }

    if (msg.Z_speed !== undefined) {
      resolved.Z_speed = msg.Z_speed;
    }
    else {
      resolved.Z_speed = 0.0
    }

    return resolved;
    }
};

module.exports = speed;
