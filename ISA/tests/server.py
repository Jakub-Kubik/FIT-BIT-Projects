# Server is used from: https://gist.github.com/bradmontgomery/2219997 
# https://gist.github.com/bradmontgomery/2219997

content = dict()

content.update({'//atom/1' : '''<?xml version="1.0" encoding="utf-8"?>

                                <feed xmlns="http://www.w3.org/2005/Atom">
                        
                                    <title>Example Feed</title>
                                    <subtitle>A subtitle.</subtitle>
                                    <link href="http://example.org/feed/" rel="self" />
                                    <link href="http://example.org/" />
                                    <id>urn:uuid:60a76c80-d399-11d9-b91C-0003939e0af6</id>
                                    <updated>2003-12-13T18:30:02Z</updated>
                        
                        
                                    <entry>
                                        <title>Atom-Powered Robots Run Amok</title>
                                        <link href="http://example.org/2003/12/13/atom03" />
                                        <link rel="alternate" type="text/html" href="http://example.org/2003/12/13/atom03.html"/>
                                        <link rel="edit" href="http://example.org/2003/12/13/atom03/edit"/>
                                        <id>urn:uuid:1225c695-cfb8-4ebb-aaaa-80da344efa6a</id>
                                        <updated>2003-12-13T18:30:02Z</updated>
                                        <summary>Some text.</summary>
                                        <content type="xhtml">
                                            <div xmlns="http://www.w3.org/1999/xhtml">
                                                <p>This is the entry content.</p>
                                            </div>
                                        </content>
                                        <author>
                                            <name>John Doe</name>
                                            <email>johndoe@example.com</email>
                                        </author>
                                    </entry>
                        
                                </feed>
                            '''})


content.update({'//atom/2' : '''<?xml version="1.0" encoding="utf-8"?>

                                <feed xmlns="http://www.w3.org/2005/Atom">
                        
                                    <title>MY title</title>
                                    <subtitle>A subtitle.</subtitle>
                                    <link href="http://example.org/feed/" rel="self" />
                                    <link href="http://example.org/" />
                                    <id>urn:uuid:60a76c80-d399-11d9-b91C-0003939e0af6</id>
                                    <updated>2003-12-13T18:30:02Z</updated>
                        
                        
                                    <entry>
                                        <title>Atom-Powered Robots Run Amok</title>
                                        <link href="http://example.org/2003/12/13/atom03" />
                                        <link rel="alternate" type="text/html" href="http://example.org/2003/12/13/atom03.html"/>
                                        <link rel="edit" href="http://example.org/2003/12/13/atom03/edit"/>
                                        <id>urn:uuid:1225c695-cfb8-4ebb-aaaa-80da344efa6a</id>
                                        <updated>2003-12-13T18:30:02Z</updated>
                                        <summary>Some text.</summary>
                                        <content type="xhtml">
                                            <div xmlns="http://www.w3.org/1999/xhtml">
                                                <p>This is the entry content.</p>
                                            </div>
                                        </content>
                                        <author>
                                            <name>John Doe</name>
                                            <email>johndoe@example.com</email>
                                        </author>
                                    </entry>
                        
                                </feed>
                            '''})


content.update({'//rss1/1' : '''<?xml version="1.0"?>

                                <rdf:RDF 
                                  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" 
                                  xmlns:dc="http://purl.org/dc/elements/1.1/"
                                  xmlns="http://purl.org/rss/1.0/"
                                >
                                
                                  <channel rdf:about="http://www.xml.com/xml/news.rss">
                                    <title>XML.com</title>
                                    <link>http://xml.com/pub</link>
                                    <description>
                                      XML.com features a rich mix of information and services 
                                      for the XML community.
                                    </description>
                                
                                    <image rdf:resource="http://xml.com/universal/images/xml_tiny.gif" />
                                
                                    <items>
                                      <rdf:Seq>
                                        <rdf:li resource="http://xml.com/pub/2000/08/09/xslt/xslt.html" />
                                        <rdf:li resource="http://xml.com/pub/2000/08/09/rdfdb/index.html" />
                                      </rdf:Seq>
                                    </items>
                                
                                  </channel>
                                  
                                  <image rdf:about="http://xml.com/universal/images/xml_tiny.gif">
                                    <title>XML.com</title>
                                    <link>http://www.xml.com</link>
                                    <url>http://xml.com/universal/images/xml_tiny.gif</url>
                                  </image>
                                  
                                  <item rdf:about="http://xml.com/pub/2000/08/09/xslt/xslt.html">
                                    <title>Processing Inclusions with XSLT</title>
                                    <link>http://xml.com/pub/2000/08/09/xslt/xslt.html</link>
                                    <dc:creator>Simon St.Laurent (mailto:simonstl@simonstl.com)</dc:creator>
                                    <dc:date>13.7.1996</dc:date>
                                    <description>
                                     Processing document inclusions with general XML tools can be 
                                     problematic. This article proposes a way of preserving inclusion 
                                     information through SAX-based processing.
                                    </description>
                                  </item>
                                  
                                  <item rdf:about="http://xml.com/pub/2000/08/09/rdfdb/index.html">
                                    <title>Putting RDF to Work</title>
                                    <link>http://xml.com/pub/2000/08/09/rdfdb/index.html</link>
                                    <dc:creator>Simon St.Laurent (mailto:simonstl@simonstl.com)</dc:creator>
                                    <dc:date>13.7.1996</dc:date>
                                    <description>
                                     Tool and API support for the Resource Description Framework 
                                     is slowly coming of age. Edd Dumbill takes a look at RDFDB, 
                                     one of the most exciting new RDF toolkits.
                                    </description>
                                  </item>
                                
                                </rdf:RDF>
                            '''})


content.update({'//rss1/2' : '''<?xml version="1.0"?>

                                <rdf:RDF 
                                  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" 
                                  xmlns:dc="http://purl.org/dc/elements/1.1/"
                                  xmlns="http://purl.org/rss/1.0/"
                                >
                                
                                  <channel rdf:about="http://www.xml.com/xml/news.rss">
                                    <title>XML.com</title>
                                    <link>http://xml.com/pub</link>
                                    <description>
                                      XML.com features a rich mix of information and services 
                                      for the XML community.
                                    </description>
                                
                                    <image rdf:resource="http://xml.com/universal/images/xml_tiny.gif" />
                                
                                    <items>
                                      <rdf:Seq>
                                        <rdf:li resource="http://xml.com/pub/2000/08/09/xslt/xslt.html" />
                                        <rdf:li resource="http://xml.com/pub/2000/08/09/rdfdb/index.html" />
                                      </rdf:Seq>
                                    </items>
                                
                                  </channel>
                                  
                                  <image rdf:about="http://xml.com/universal/images/xml_tiny.gif">
                                    <title>XML.com</title>
                                    <link>http://www.xml.com</link>
                                    <url>http://xml.com/universal/images/xml_tiny.gif</url>
                                  </image>
                                  
                                  <item rdf:about="http://xml.com/pub/2000/08/09/xslt/xslt.html">
                                    <title>Processing Inclusions with XSLT</title>
                                    <link>http://xml.com/pub/2000/08/09/xslt/xslt.html</link>
                                    <description>
                                     Processing document inclusions with general XML tools can be 
                                     problematic. This article proposes a way of preserving inclusion 
                                     information through SAX-based processing.
                                    </description>
                                  </item>
                                  
                                  <item rdf:about="http://xml.com/pub/2000/08/09/rdfdb/index.html">
                                    <title>Putting RDF to Work</title>
                                    <link>http://xml.com/pub/2000/08/09/rdfdb/index.html</link>
                                    <dc:creator>Simon St.Laurent (mailto:simonstl@simonstl.com)</dc:creator>
                                    <dc:date>13.7.1996</dc:date>
                                    <description>
                                     Tool and API support for the Resource Description Framework 
                                     is slowly coming of age. Edd Dumbill takes a look at RDFDB, 
                                     one of the most exciting new RDF toolkits.
                                    </description>
                                  </item>
                                
                                </rdf:RDF>
                            '''})


content.update({'//rss2/1' : '''<?xml version="1.0" encoding="UTF-8" ?>
                                <rss version="2.0">
                                    <channel>
                                         <title>RSS Title</title>
                                         <description>This is an example of an RSS feed</description>
                                         <link>http://www.example.com/main.html</link>
                                         <lastBuildDate>Mon, 06 Sep 2010 00:01:00 +0000 </lastBuildDate>
                                         <pubDate>Sun, 06 Sep 2009 16:20:00 +0000</pubDate>
                                         <ttl>1800</ttl>
                                        
                                         <item>
                                              <title>Example entry</title>
                                              <author>Johny Mrkvicka</author>
                                              <description>Here is some text containing an interesting description.</description>
                                              <link>http://www.example.com/blog/post/1</link>
                                              <guid isPermaLink="false">7bd204c6-1655-4c27-aeee-53f933c5395f</guid>
                                              <pubDate>Sun, 06 Sep 2009 16:20:00 +0000</pubDate>
                                         </item>
                                         <item>
                                              <title>Next header</title>
                                              <description>Here is some text containing an interesting description.</description>
                                              <link>http://www.example.com/blog/post/1</link>
                                              <guid isPermaLink="false">7bd204c6-1655-4c27-aeee-53f933c5395f</guid>
                                         </item>
                                        
                                    </channel>
                                </rss>
                            '''})


content.update({'//rss2/2' : '''<?xml version="1.0" encoding="UTF-8" ?>
                                <rss version="2.0">
                                    <channel>
                                         <title>RSS Title</title>
                                         <description>This is an example of an RSS feed</description>
                                         <link>http://www.example.com/main.html</link>
                                         <lastBuildDate>Mon, 06 Sep 2010 00:01:00 +0000 </lastBuildDate>
                                         <pubDate>Sun, 06 Sep 2009 16:20:00 +0000</pubDate>
                                         <ttl>1800</ttl>
                                        
                                         <item>
                                              <title>Example entry</title>
                                              <author>Johny Mrkvicka</author>
                                              <description>Here is some text containing an interesting description.</description>
                                              <link>http://www.example.com/blog/post/1</link>
                                              <guid isPermaLink="false">7bd204c6-1655-4c27-aeee-53f933c5395f</guid>
                                              <pubDate>Sun, 06 Sep 2009 16:20:00 +0000</pubDate>
                                         </item>
                                         <item>
                                              <title>Next header</title>
                                              <description>Here is some text containing an interesting description.</description>
                                              <link>http://www.example.com/blog/post/1</link>
                                              <guid isPermaLink="false">7bd204c6-1655-4c27-aeee-53f933c5395f</guid>
                                         </item>
                                        
                                    </channel>
                                </rss>
                            '''})

import time
from http.server import BaseHTTPRequestHandler, HTTPServer

HOST_NAME = 'localhost'
PORT_NUMBER = 5300


class MyHandler(BaseHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/xml')
        self.end_headers()

    def do_GET(self):
        # handle a favicon.ico default 
        # GET simple http server request 
        if self.path == '/favicon.ico':
            return

        self.send_response(200)
        self.send_header('Content-type', 'text/xml')
        self.end_headers()

        response = bytes(content[self.path], 'UTF-8')
        self.wfile.write(response)


    
if __name__ == '__main__':
    server_class = HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    #print(time.asctime(), 'Server Starts - %s:%s' % (HOST_NAME, PORT_NUMBER))
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
#print(time.asctime(), 'Server Stops - %s:%s' % (HOST_NAME, PORT_NUMBER))