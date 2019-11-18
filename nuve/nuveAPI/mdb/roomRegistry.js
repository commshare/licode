/* global require, exports, ObjectId */


const db = require('./dataBase').db;

const logger = require('./../logger').logger;

// Logger
const log = logger.getLogger('RoomRegistry');
//mongodb debugging - ReferenceError: console is not defined
//var console = {};
//console.log = print;

exports.getRooms = (callback) => {
  db.rooms.find({}).toArray((err, rooms) => {
    if (err || !rooms) {
      log.info('message: rooms list empty');
    } else {
      callback(rooms);
    }
  });
};

const getRoom = (id, callback) => {
  db.rooms.findOne({ _id: db.ObjectId(id) }, (err, room) => {
    if (room === undefined) {
      log.warn(`message: getRoom - Room not found, roomId: ${id}`);
    }
    if (callback !== undefined) {
      callback(room);
    }
  });
};

exports.getRoom = getRoom;

const hasRoom = (id, callback) => {
  getRoom(id, (room) => {
    if (room === undefined) {
      callback(false);
    } else {
      callback(true);
    }
  });
};

exports.hasRoom = hasRoom;
/*
 * Adds a new room to the data base.
 */
exports.addRoom = (room, callback) => {
  db.rooms.save(room, (error, saved) => {
    if (error) log.warn(`message: addRoom error, ${logger.objectToLog(error)}`);
    callback(saved);
  });
};
//
// /* eslint-disable */
// exports.assignErizoControllerToRoom = function(room, erizoControllerId, callback) {
//   log.warn(`assignErizoControllerToRoom:  erizoControllerId`,erizoControllerId);
//   return db.eval(function(id, erizoControllerId) {
//     log.warn(`[ROOMRG]db eval --begin--`);
//     var erizoController;
//     var room = db.rooms.findOne({_id: new ObjectId(id)});
//     if (!room) {
//       log.error(`[ROOMRG]db rooms findone fail ,no room `);
//       return erizoController;
//     }
//
//     if (room.erizoControllerId) {
//       erizoController = db.erizoControllers.findOne({_id: room.erizoControllerId});
//       if (erizoController) {
//         log.error(`[ROOMRG]db rooms findone fail ,no room `);
//         return erizoController;
//       }
//     }
//
//     erizoController = db.erizoControllers.findOne({_id: new ObjectId(erizoControllerId)});
//
//     if (erizoController) {
//       room.erizoControllerId = new ObjectId(erizoControllerId);
//       log.error(`[ROOMRG]db rooms  save room `);
//       db.rooms.save( room );
//     }
//     log.warn(`[ROOMRG]db eval --end???--`);
//     return erizoController;
//   }, room._id + '', erizoControllerId + '', function(error, erizoController) {
//     log.warn(`[ROOMRG]======1===`);
//     if (error) log.warn('find err: assignErizoControllerToRoom error, ' + logger.objectToLog(error));
//     if (callback) {
//       log.warn('[ROOMRG] ---begin assignErizoControllerToRoom:  callback, ');
//       callback(erizoController);
//       log.warn('[ROMMRG] ---end assignErizoControllerToRoom:  callback, ');
//
//     }else
//     {
//       log.warn(`[ROOMRG]======no assignErizoControllerToRoom callback==`);
//
//     }
//   });
// };

// /* eslint-disable */
// exports.assignErizoControllerToRoom = function(room, erizoControllerId, callback) {
//   console.log(`assignErizoControllerToRoom:  erizoControllerId`,erizoControllerId);
//   return db.eval(function(id, erizoControllerId) {
//     console.log(`[ROOMRG]db eval --begin--`);
//     var erizoController;
//     var room = db.rooms.findOne({_id: new ObjectId(id)});
//     if (!room) {
//       console.log(`[ROOMRG]db rooms findone fail ,no room `);
//       return erizoController;
//     }
//
//     if (room.erizoControllerId) {
//       erizoController = db.erizoControllers.findOne({_id: room.erizoControllerId});
//       if (erizoController) {
//         console.log(`[ROOMRG]db rooms findone fail ,no room `);
//         return erizoController;
//       }
//     }
//
//     erizoController = db.erizoControllers.findOne({_id: new ObjectId(erizoControllerId)});
//
//     if (erizoController) {
//       room.erizoControllerId = new ObjectId(erizoControllerId);
//       console.log(`[ROOMRG]db rooms  save room `);
//       db.rooms.save( room );
//     }
//     console.log(`[ROOMRG]db eval --end???--`);
//     return erizoController;
//   }, room._id + '', erizoControllerId + '', function(error, erizoController) {
//     console.log(`[ROOMRG]======1===`);
//     if (error) console.log('find err: assignErizoControllerToRoom error, ' + logger.objectToLog(error));
//     if (callback) {
//       console.log('[ROOMRG] ---begin assignErizoControllerToRoom:  callback, ');
//       callback(erizoController);
//       console.log('[ROMMRG] ---end assignErizoControllerToRoom:  callback, ');
//
//     }else
//     {
//       console.log(`[ROOMRG]======no assignErizoControllerToRoom callback==`);
//
//     }
//   });
// };



/* eslint-disable */
exports.assignErizoControllerToRoom = function(room, erizoControllerId, callback) {
  return db.eval(function(id, erizoControllerId) {
    var erizoController;
    var room = db.rooms.findOne({_id: new ObjectId(id)});
    if (!room) {
      return erizoController;
    }

    if (room.erizoControllerId) {
      erizoController = db.erizoControllers.findOne({_id: room.erizoControllerId});
      if (erizoController) {
        return erizoController;
      }
    }

    erizoController = db.erizoControllers.findOne({_id: new ObjectId(erizoControllerId)});

    if (erizoController) {
      room.erizoControllerId = new ObjectId(erizoControllerId);

      db.rooms.save( room );
    }
    return erizoController;
  }, room._id + '', erizoControllerId + '', function(error, erizoController) {
    if (error) log.warn('message: assignErizoControllerToRoom error, ' + logger.objectToLog(error));
    if (callback) {
      callback(erizoController);
    }
  });
};

/* eslint-enable */

/*
 * Updates a determined room
 */
exports.updateRoom = (id, room, callback) => {
  db.rooms.update({ _id: db.ObjectId(id) }, room, (error) => {
    if (error) log.warn(`message: updateRoom error, ${logger.objectToLog(error)}`);
    if (callback) callback(error);
  });
};

/*
 * Removes a determined room from the data base.
 */
exports.removeRoom = (id) => {
  hasRoom(id, (hasR) => {
    if (hasR) {
      db.rooms.remove({ _id: db.ObjectId(id) }, (error) => {
        if (error) {
          log.warn(`message: removeRoom error, ${logger.objectToLog(error)}`);
        }
      });
    }
  });
};
