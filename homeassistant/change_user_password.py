# https://community.home-assistant.io/t/password-lost/66288/13
# Password lost:
# Shut down HA; Replace your password in auth_provider.homeassistant with the password you got from python

import bcrypt
import base64
password = "secret"
hashed = bcrypt.hashpw(password.encode(), bcrypt.gensalt(rounds=12))
hashed = base64.b64encode(hashed)
print(hashed)
