<h2>Part 1. Настройка gitlab-runner</h2>

<p>Поднимаем виртуальную машину Ubuntu Server 22.04 LTS.</p>
    <img src ="ckrinshoti/2.png">
<p>Скачиваем и устанавливаем gitlab-runner на виртуальную машину
</p> <p>1. Добавляем репозиторий GitLab Runner:</p>
<code>curl -L https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.deb.sh | sudo bash</code>
    <img src ="ckrinshoti/3.png">
<p>2. Дать файлу разрешение на исполнение</p>
<code>sudo chmod +x /usr/local/bin/gitlab-runner</code>
<p>3. Установите GitLab Runner:
</p>
<code>sudo apt-get install gitlab-runner
</code>
<p>4. Создать пользователя GitLab CI</p>
<code>sudo useradd --comment 'GitLab Runner' --create-home gitlab-runner --shell /bin/bash</code>
<p>5. Запустить службу</p>
<code>sudo gitlab-runner start</code>
<p>Запускаем и регистрируем gitlab-runner для использования в текущем проекте (DO6_CICD) </p>
    <img src ="ckrinshoti/1.png">
<code>gitlab-runner register</code>
<p>Далее прописываем URL, токен, описание раннера (напр. DO6_CICD), два следующих оставляем пустыми, в строке executor прописываем <code>shell</code></p>
<h2>Part 2. Сборка</h2>
<p>Создаем файл <code>.gitlab-ci.yml</code> в корне репозитория</p>
    <img src ="ckrinshoti/1.1.png">
<p>Также добавляем директории cat и grep в src</p>
<p>Проводим сборку</p>
    <img src ="ckrinshoti/8.png">
<h2>Part 3. Тест кодстайла</h2>
<p>Hапиcать этап в ямлике .gitlab-ci.yml, который запускает скрипт кодстайла (clang-format)</p>
    <img src ="ckrinshoti/9.png">
<p>Запускаем сборку</p>
    <img src ="ckrinshoti/10.png">
<h2>Part 4. Интеграционные тесты</h2>
<p>Hапиcать этап в ямлике .gitlab-ci.yml, который запускает интаграционные тесты</p>
    <img src ="ckrinshoti/11.png">
<p>Запускаем сборку</p>
    <img src ="ckrinshoti/12.png">
<h2>Part 5. Этап деплоя</h2>
<p>Создаем вторую вертуальную машину</p>
    <img src ="ckrinshoti/13.png">
<p>Прописываем каждой машине ip адрес в netplan. 
На машине, на которой установлен gitlab-runner, создаем ssh ключ от пользователя gitlab-runner</p>
<code>sudo su gitlab-runner</code>
<code>ssh-keygen</code>
<p>Затем копируем открытый ssh ключ на удаленный сервер и подключаемя к серверу</p>
<code>ssh-copy-id [nickname]@[ip addr]</code>
<code>ssh [nickname]@[ip addr]</code>
<p>Hапиcать этап в ямлике .gitlab-ci.yml, который запускает deploy</p>
    <img src ="ckrinshoti/1.3.png">
<p>Запускаем сборку, деплой запускается отдельно</p>
    <img src ="ckrinshoti/14.png">
<h2>Part 6. Дополнительно. Уведомления</h2>
<p>bash скрипт для отправления уведомлений</p>
    <img src ="ckrinshoti/15.png">
<p>Добавляем выполнение скрипта после каждого этапа
</p>
    <img src ="ckrinshoti/16.png">
<p>В телеграмме получаем уведомления</p>
    <img src ="ckrinshoti/17.png">