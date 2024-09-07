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

class position3d {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.x3d = null;
      this.y3d = null;
      this.z3d = null;
    }
    else {
      if (initObj.hasOwnProperty('x3d')) {
        this.x3d = initObj.x3d
      }
      else {
        this.x3d = 0;
      }
      if (initObj.hasOwnProperty('y3d')) {
        this.y3d = initObj.y3d
      }
      else {
        this.y3d = 0;
      }
      if (initObj.hasOwnProperty('z3d')) {
        this.z3d = initObj.z3d
      }
      else {
        this.z3d = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type position3d
    // Serialize message field [x3d]
    bufferOffset = _serializer.int64(obj.x3d, buffer, bufferOffset);
    // Serialize message field [y3d]
    bufferOffset = _serializer.int64(obj.y3d, buffer, bufferOffset);
    // Serialize message field [z3d]
    bufferOffset = _serializer.int64(obj.z3d, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type position3d
    let len;
    let data = new position3d(null);
    // Deserialize message field [x3d]
    data.x3d = _deserializer.int64(buffer, bufferOffset);
    // Deserialize message field [y3d]
    data.y3d = _deserializer.int64(buffer, bufferOffset);
    // Deserialize message field [z3d]
    data.z3d = _deserializer.int64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 24;
  }

  static datatype() {
    // Returns string type for a message object
    return 'depthcam/position3d';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '0c2f0543d8404bbb5cf654877be20b1f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int64 x3d
    int64 y3d
    int64 z3d
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new position3d(null);
    if (msg.x3d !== undefined) {
      resolved.x3d = msg.x3d;
    }
    else {
      resolved.x3d = 0
    }

    if (msg.y3d !== undefined) {
      resolved.y3d = msg.y3d;
    }
    else {
      resolved.y3d = 0
    }

    if (msg.z3d !== undefined) {
      resolved.z3d = msg.z3d;
    }
    else {
      resolved.z3d = 0
    }

    return resolved;
    }
};

module.exports = position3d;
