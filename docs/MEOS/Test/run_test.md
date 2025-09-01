# Tests

## Template for Tests

- Navigate to the `Tests/Functions` folder.  
- Ensure that any new function tests follow the **same template** as the existing ones.  
- Maintain **naming consistency** with the MEOS functions when creating new tests.  

## Running the Tests

1. Make sure the build is successful (see **Setup Instructions**).  
2. From the **workspace directory**, run the following command inside the Docker container:  

   ```bash
   ./build/nes-systests/systest/systest -t Tests/example.test
