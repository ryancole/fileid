
// node modules
var fs = require('fs');

// local modules
var fileid = require('../build/Release/fileid');


function initialize (callback) {
    
    return fileid.initialize(callback);
    
};

function identify (path, callback) {
    
    // initialize the file id engine
    initialize(function (err, success) {
        
        // error on failure
        if (err) return callback(err);
        
        // error on failure
        if (!success) return callback(new Error('failed to initialize file id engine'));
        
        // get information about the given file
        fs.stat(path, function (err, stats) {
            
            // error on failure
            if (err) return callback(err);
            
            // error on non file
            if (!stats.isFile()) return callback(new Error('path is not a file'));
            
            // identify the given file
            fileid.identify(path, function (err, details) {
                
                // error on failure
                if (err) return callback(err);
                
                // file details on success
                return callback(null, details);
                
            });
            
        });
        
    });
    
};

// module exports
exports.identify = identify;
exports.initialize = initialize;