# CIA+AAA

## Confidentiality

- Users can only access their own payment info
- Users can only purchase images for themselves.

## Integrity

- Administrators can edit or delete the details of images uploaded by users.
- Users can view and edit the details of images they have uploaded. 

## Availability

- Users can view and download all images.
- Administrators can view and download all images.

## Authentication

- Users can create and register a simple account.
- Administrator accounts are created at deployment.
- No user identification is required to view or download images.
- In order to access functions besides downloading and viewing images, logging in is required.
- Following a successful login users are given a unique session id.
- Followin a logout, the user's active session id is invalidated.

## Authorization

- Functions available without a role: view and download images.
- Functions accessible with user role: buy and sell images, upload and download images
- Functions accessible with admin role: buy and sell images, upload and download images, edit and delete image data

## Auditing

- Logging in/out and registration of users is logged
- Upload events of images are logged.
- Writing, deleting and editing comments are logged.
- Payment events are logged with the corresponding userId, but no payment info.
