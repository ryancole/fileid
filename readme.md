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

# Compiling Prerequisites

Download the Oracle file id SDK, place the header files in `/usr/local/include` and the shared objects in `/usr/local/lib`. Make sure `/usr/local/lib` is in `$LD_LIBRARY_PATH`.

# Oracle

You can download Oracle's tools at the following URL: http://www.oracle.com/technetwork/middleware/content-management/downloads/oit-dl-otn-097435.html