
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
        var details = fileid.identify(path);
        
        // file details on success
        if (details)
            return callback(null, details);
        
        return callback(new Error('failed to identify file'));
        
    });
    
};
