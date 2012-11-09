
var fileid = require('../lib/fileid');

fileid.identify('/home/ryan/KPMG-CVSLONGS-00005455.XLS', function (err, details) {
    
    console.log(details);
    
});
