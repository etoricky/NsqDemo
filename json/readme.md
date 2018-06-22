C# JSON
=======

    {
      "Id": "d69d7de3-1219-4fe6-9dbf-89e5866b9c26",
      "TemplateID": "2760F1A1-0DC8-48B5-B9F2-29144E0C5CC9",
      "KeyValues": [
        {
          "field": "{Title}",
          "value": "Mrs"
        },
        {
          "field": "{LastName}",
          "value": "Let"
        },
        {
          "field": "{VerifyUrl}",
          "value": "http://baidu.com"
        }
      ],
      "SendTo": "abssdf@rhyta.com",
      "Subject": "Verify your email address"
    }

Explain
=======

    Id:每次推送时new guid 
    TemplateID：这个是提供的模板id ，程序配置
    KeyValues：这个是邮件模析内容定好后，看需求那些值是要动态变换的，看最终需求再定
    SendTo：邮箱
    Subject：标题，配置项
    
    
C#
==

    public class EmailMessage
    {
        public EmailMessage()
        {
            KeyValues = new List<KeyValue>();
        }
        public Guid Id { get; set; } = Guid.NewGuid();
        public string TemplateID { get; set; }
        public List<KeyValue> KeyValues { get; set; }
        public string SendTo { get; set; }
        public string Subject { get; set; }
        public List<string> AttachmentAddress { get; set; }
    }
    public class KeyValue
    {
        public string field { get; set; }
        public string value { get; set; }
    }
    
Python json
===========

    {
      "Id": "d69d7de3-1219-4fe6-9dbf-89e5866b9c26",
      "TemplateID": "2760F1A1-0DC8-48B5-B9F2-29144E0C5CC9",
      "KeyValues": [
        {
          "field": "{Title}",
          "value": "Mrs"
        },
        {
          "field": "{LastName}",
          "value": "Let"
        },
        {
          "field": "{VerifyUrl}",
          "value": "http://baidu.com"
        }
      ],
      "SendTo": "abssdf@rhyta.com",
      "Subject": "Verify your email address"
    }

C++ nlohmann json
=================

    {
      "Id": "d69d7de3-1219-4fe6-9dbf-89e5866b9c26",
      "KeyValues": [
        {
          "field": "{Title}",
          "value": "Mrs"
        },
        {
          "field": "{LastName}",
          "value": "Let"
        },
        {
          "field": "{VerifyUrl}",
          "value": "http://baidu.com"
        }
      ],
      "SendTo": "abssdf@rhyta.com",
      "Subject": "Verify your email address",
      "TemplateID": "2760F1A1-0DC8-48B5-B9F2-29144E0C5CC9"
    }