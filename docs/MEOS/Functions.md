# Functions Parameters

**Standard functions, such as functions used for filtering and mapping, can have two types:**

- Fixed Geometry/geography as input. The input can be Stbox, Polygon, Point, and so on. The function will have four arguments :
    - Function (double streamLatitude, double streamLongitude, long streamTS,  String fixedGeom)
- No ‘external’ argument, all elements come from streams. In this case, we can have two streams that were merged. For example, the weather and the train positions. The function will have six arguments:
    - Function (double weatherLat, double weatherLongitude, long weatherTS, double trainLat, double trainLongitude, long trainTS)

**Aggregation functions (used in .apply) that combine all elements from the window have three arguments:**

- No ‘external’ argument, all elements come from the window.
    - Function (double streamLatitude, double streamLongitude, long streamTS)
- Aggregation functions are split into three subfunctions following the MapReduce paradigm:
    - Lift - where the function is applied to each element
    - Combine - where the result of the lift is combined, two by two
    - Lower - where the final function is applied
        - For example, a sum of speed:
            - Lift will read the speed  for each element in the window
            - Combine will do the actual sum,  two by two elements
            - Lower will only return the result
            - We have one sum per window
        - For example, a mean of speeds:
            - Lift will read the speed  for each element in the window
            - Combine will do the actual sum, two by two elements
            - Lower will divide the sum by the number of elements and return the result
            - We have one mean per window

# Functions Location
Functions should be located in MEOS directories in each corresponding layer

# Adding a Functions
- Logical layer
- Physical layer
- MEOSWrapper layer
- SQL layer