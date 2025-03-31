.global halt
.type halt, @function
halt:
    b .
    
.size halt, (. - halt)
