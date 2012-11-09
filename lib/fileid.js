
// node modules
var fs = require('fs');

// local modules
var fileid = require('../build/Release/fileid');


exports.identify = function identify (path, callback) {
    
    fs.stat(path, function (err, stats) {
        
        // error on failure
        if (err)
            return callback(err);
        
        // error on non file
        if (!stats.isFile())
            return callback(new Error('path is not a file'));
        
        // identify the given file
        fileid.identify(path, function (err, details) {
            
            // failure on error
            if (err)
                return callback(new Error('failed to identify file'));
            
            // file details on success
            return callback(null, details);
            
        });
        
    });
    
};
