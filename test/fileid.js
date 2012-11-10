
var fileid = require('../lib/fileid');

fileid.identify('./test/test.xlsx', function (err, details) {
    
    if (err)
        console.log(err);
    
    console.log(details);
    
});
