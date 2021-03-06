<?php
/*
 *  This is a email back end module using the Official Twilio SendGrid
 *  Led, Community Driven PHP API Library
 *  https://sendgrid.com
 */

/*.
    require_module 'standard';
  .*/

namespace ciab\backend;

class Email
{


    public function __construct()
    {

    }


    public function mail(
        $to,
        $from,
        $subject,
        $message,
        $replyTo = null,
        $type = null
    ) {
        if ($from == null) {
            $from = $GLOBALS['SENDGRID_MAIL_FROM'];
        }
        $key = $GLOBALS['SENDGRID_API_KEY'];
        if ($from == null || $key == null) {
            error_log("ERROR: Sendgrid e-mail not configured correctly! SENDGRID_MAIL_FROM and/or SENDGRID_API_KEY undefined "."in .env file");
            return false;
        }

        $email = new \SendGrid\Mail\Mail();
        $email->setFrom($from);
        $email->setSubject($subject);
        if ($replyTo !== null) {
            $email->setReplyTo($replyTo);
        }
        $email->addTo($to);
        if ($type === null) {
            $type = "text/plain";
        }
        $email->addContent($type, $message);
        $sendgrid = new \SendGrid($GLOBALS['SENDGRID_API_KEY']);
        try {
            $response = $sendgrid->send($email);
            if ($response->statusCode() == 200 ||
                $response->statusCode() == 202) {
                return true;
            }
            error_log("SENDGRID Failure: ".$response->statusCode().": ");
            error_log(print_r($response->headers(), true));
        } catch (Exception $e) {
            error_log('SENDGRID: Caught exception: '.$e->getMessage()."\n");
        }
        return false;

    }


    /* end class */
}
