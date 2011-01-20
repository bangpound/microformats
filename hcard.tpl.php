<div class="vcard">
  <div class="n">
    <span class="given-name"><?php print $data['n']['given-name']; ?></span>
    <span class="additional-name"><?php print $data['n']['additional-name']; ?></span>
    <span class="family-name"><?php print $data['n']['family-name']; ?></span>
  </div>
  <div class="org"><?php print $data['org']; ?></div>
  <a class="email" href="mailto:<?php print $data['email']; ?>"><?php print $data['email']; ?></a>
  <a class="url" href="<?php print $data['url']; ?>"><?php print $data['url']; ?></a>
  <div class="adr"><?php print $data['adr']; ?></div>
  <div class="tel"><?php print $data['tel']; ?></div>
</div>
