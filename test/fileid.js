
var fileid = require('../lib/fileid');

// identify a single file
fileid.identify('./test/etc/test.xlsx', function (err, details) {
    
    if (err) {
        
        console.log(err);
        
    } else {
        
        console.log(details);
        
    }
    
});
