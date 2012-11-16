
// node modules
var fs = require('fs');

// local modules
var fileid = require('../build/Release/fileid');


function identify (path, callback) {
    
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
    
};

// module exports
exports.identify = identify;