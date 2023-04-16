# Security Test Plan

## User privilige tests:
- Test that "regular" users can only access the functions they are allowed to access and cannot modify or delete data.
- Check that administrators are only allowed to modify or delete users and products that they have authorised.
- Test that users cannot gain unauthorised access to data or functions stored in the system.

## Input validation tests:
- Test that the application correctly handles user input and prevents SQL injection and cross-site scripting (XSS) attacks.
- Check that the size and type of uploaded files meet the specified restrictions and do not contain malicious code.
- Test that the application handles incorrect input and displays appropriate error messages to users.

## Server-side security tests:
- Test that the application properly handles errors and does not allow access from unauthorized users.
- Test that the application properly restricts access to the database and does not allow unauthorized users to modify or delete data.
- Check server-side configuration and settings to ensure that the application is properly protected from phishing, attacks and other security threats.

## Mobile and web client tests:
- Test that clients handle data and user interactions properly.
Check that clients are not sending sensitive data to the server, such as passwords or personal information.
- Test that clients are using encryption and SSL/TLS properly when communicating with the server.
