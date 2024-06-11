import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

def SendMail(alert_position):
 print('Sending Email')
 email_sender = 'madhubootcamp@gmail.com'
 email_receiver = 'sreekantht997@gmail.com'
 subject = 'WASP position alert!'
 msg = MIMEMultipart()
 msg['From'] = email_sender
 msg['To'] = email_receiver
 msg['Subject']= subject
 body = str(f'panic alert detected near position:-{alert_position}')
 msg.attach(MIMEText(body, 'plain'))

 text = msg.as_string()
 connection = smtplib.SMTP('smtp.gmail.com', 587)
 connection.starttls()
 connection.login(email_sender, 'autoMAD0')
 connection.sendmail(email_sender, email_receiver, text )
 connection.quit()
 print('Email Sent')