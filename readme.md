A node.js addon for Oracle's file id library.

# Usage

```javascript
var fileid = require('fileid');

fileid.identify('/home/foo/bar.xls', function (err, details) {

    console.log(details);

});
```

# Output

```javascript
{ id: 1446, name: 'Microsoft Excel 2000' }
```

# Oracle

You can download Oracle's tools at the following URL: http://www.oracle.com/technetwork/middleware/content-management/downloads/oit-dl-otn-097435.html