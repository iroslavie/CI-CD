#!/bin/bash

sudo scp -i /home/gitlab-runner/.ssh/id_rsa src/cat/s21_cat laurettw@192.168.10.2:/tmp
sudo scp -i /home/gitlab-runner/.ssh/id_rsa src/grep/s21_grep laurettw@192.168.10.2:/tmp

