# Threat modelling with STRIDE

## Spoofing:

- Attackers might try to impersonate a legitimate user to gain access to their payment information.
- Attackers might try to impersonate a legitimate user to sell their owned images to themselves.
 
## Tampering:
 
- Attackers might try to modify the CAFF or CIFF files during upload to include malicious code in the meta-data.
- Attackers might try to modify the meta-data associated with the files to misrepresent their contents.
 
## Repudiation:
 
- Attackers might deny that they uploaded or purchased specific images.
- Attackers might deny that they made specific comments on an image.
- Attackers might deny that they received the money for sold images.
 
## Information Disclosure:
 
- Attackers might try to access and steal sensitive user data, such as login credentials, personal information, and payment information via injection attacks against the AuthServer.
- Attackers might try to gain access payment information by MITM attacking the paymentServer and the Bank.
 
## Denial of Service:
 
- Attackers might try to overload the servers by making too many requests.
- Attackers might try to flood the system with uploaded images and comments.
- Attackers might try to trade pictures between themselves to bring the servers down.
 
## Elevation of Privilege:
 
- Attackers might try to gain admin privileges via injection attacks against the AuthServer.
