#!/usr/bin/python
from atproto import Client, client_utils
import sys

if 4 == len(sys.argv):
  client = Client()
  profile = client.login(sys.argv[1], sys.argv[2])

  text = client_utils.TextBuilder().text(sys.argv[3])
  post = client.send_post(text)
else:
	print("Needs all of the fields to post")
